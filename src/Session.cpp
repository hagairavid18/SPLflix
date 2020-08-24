//
// Created by naglera on 22/11/2019.
//
#include "../include/Session.h"
#include <iostream>
#include <unordered_map>
#include <cstdlib>
#include "../include/json.hpp"
#include <fstream>
#include <vector>
#include <string>
#include "../include/Watchable.h"
#include "../include/User.h"
#include "../include/Action.h"
using namespace std;

 std::unordered_map<std::string,User*>& Session::getMap()  { return userMap; }

std:: vector<BaseAction*> Session::getActionLogVec() { return actionsLog;}

User* Session::getActiveUser() { return activeUser;}

void Session::setActiveLog(BaseAction *action) {
    actionsLog.push_back(action);
}

Session::Session():content(),actionsLog(),userMap(),activeUser() {}

void Session::copy(const Session &other) {
    for (unsigned int i = 0; i <other.content.size() ; ++i) {
        this->content.push_back(other.content[i]->clone());
    }
    for (unsigned int j = 0; j < other.actionsLog.size(); ++j) {
        actionsLog.push_back(other.actionsLog[j]->clone());
    }
    for (auto it=other.userMap.begin();it!=other.userMap.end();it++){
        userMap[it.operator->()->first]=it.operator->()->second->clone(*this);
    }


    this->activeUser = userMap[other.activeUser->getName()];

}//private

void Session::clean() {
        for (auto it = userMap.begin(); it != userMap.end(); it++) {
            delete (it.operator->()->second);
        }
        for (unsigned int i = 0; i < content.size(); ++i) {
            delete (content[i]);
        }
        for (unsigned int j = 0; j < actionsLog.size(); ++j) {
            delete (actionsLog[j]);
        }
        userMap.clear();
        content.clear();
        actionsLog.clear();

}//private

//rule of five:



Session::~Session() {
    clean();
}
Session::Session(const Session &other):Session()  {//copy constructor
    this->copy(other);
}
Session& Session::operator=(const Session &other) {//copy assignment operator
    if (this!=&other) {
        this->clean();
        this->copy(other);
    }
    return *this;
}
Session::Session(Session &&other):Session() {//move constructor
    for (unsigned int i = 0; i <other.content.size() ; ++i) {
        content.push_back(other.content[i]);
        other.content[i]= nullptr;
    }
    for (unsigned int j = 0; j < other.actionsLog.size(); ++j) {
        actionsLog.push_back(other.actionsLog[j]);
        other.actionsLog[j] = nullptr;
    }
    for (auto it = other.userMap.begin(); it != other.userMap.end(); it++) {
        userMap[it.operator->()->first] = it.operator->()->second;
    }
    this->activeUser = userMap[other.activeUser->getName()];



    for (auto it = other.userMap.begin(); it != other.userMap.end(); it++) {
        other.userMap[it.operator->()->first] = nullptr;
    }


    other.activeUser = nullptr;
}
Session& Session::operator=(Session &&other) {//move assignment operator
    if (this!=&other) {
        this->clean();
        for (unsigned int i = 0; i < other.content.size(); ++i) {
            content.push_back(other.content[i]);
            other.content[i] = nullptr;
        }
        for (unsigned int j = 0; j < other.actionsLog.size(); ++j) {
            actionsLog.push_back(other.actionsLog[j]);
            other.actionsLog[j] = nullptr;
        }
        for (auto it = other.userMap.begin(); it != other.userMap.end(); it++) {
            userMap[it.operator->()->first] = it.operator->()->second;
        }
        this->activeUser = userMap[other.activeUser->getName()];



        for (auto it = other.userMap.begin(); it != other.userMap.end(); it++) {
            other.userMap[it.operator->()->first] = nullptr;
        }


        other.activeUser = nullptr;

    }
    return *this;
}



Session::Session(const std::string &configFilePath) :Session() {
    LengthRecommenderUser *defaultUser=new LengthRecommenderUser("default");
    userMap["default"] = defaultUser;
    activeUser = defaultUser;
    std::fstream ifs(configFilePath);
    nlohmann::json j;
    ifs >> j;

    int id = 0;
    int length;
    std::string name;
    std::vector<std::string> tags;
    std::vector<int> seasons;


    auto movies = j["movies"].begin();
    while (movies != j["movies"].end()) {
        nlohmann::json each_movie = movies.value();
        length = each_movie["length"];
        name = each_movie["name"];
        std::vector<std::string> tags = each_movie["tags"];

        Movie *m1 = new Movie(id, name, length, tags); //insert each movie into content vector;
        content.push_back(m1);
        movies++;
        id++;
    }
    auto episodes = j["tv_series"].begin();
    while (episodes != j["tv_series"].end()) {
        nlohmann::json each_episode = episodes.value();
        length = each_episode["episode_length"];
        name = each_episode["name"];
        std::vector<std::string> tags = each_episode["tags"];
        std::vector<int> seasons = each_episode["seasons"];
        for (unsigned int i = 1; i <= seasons.size(); ++i) {
            for (int k = 1; k <= seasons[i-1]; ++k) {


                Episode *(e1) = new Episode(id, name, length, i, k, tags); //insert each movie into content vector;
                content.push_back(e1);
                id++;
            }
        }
        episodes++;
    }
}

const std::vector<Watchable*>& Session::getContent() const {
    return content;
}

void Session::setActiveUser(User *newUser) {
    activeUser=newUser;
}

void Session::start() {
    std::cout << "SPLFLIX is now on!" << std::endl;
    std::vector<std::string> Actions{"createuser", "changeuser", "deleteuser", "dupuser", "content", "watchhist", "watch", "log", "exit"};

    std::string action;




    unsigned int i;
    do {
        std::cin >> action;
        for (i = 0; i < Actions.size(); ++i) { //determine which action is being take

            if (action == Actions[i]) break;
        }

        switch (i) {


            case 0: {//CreateUser was called

                std:: string name;
                std:: string rec;
                std:: cin >> name;
                std:: cin >> rec;
                CreateUser *newUser= new CreateUser(name,rec);
                newUser->act(*this);
                setActiveLog(newUser);

            }
                break;


            case 1: {//Change Active User

                std::string name;
                std::cin >> name;
                ChangeActiveUser *changeUser = new ChangeActiveUser(name);
                changeUser->act(*this);
                setActiveLog(changeUser);
            }
                break;
            case 2: { //Delete User
                std::string name;
                std::cin >> name;
                DeleteUser *deleteUser=new DeleteUser(name);
                deleteUser->act(*this);
                setActiveLog(deleteUser);
            }
                break;

            case 3:{//DuplicateUser
                std::string name;
                std::string otherName;
                std::cin >> name;
                std::cin >> otherName;
                DuplicateUser *duplicateUser=new DuplicateUser(otherName,name);
                duplicateUser->act(*this);

                setActiveLog(duplicateUser);

            }
                break;

            case 4:{//PrintContentList
                PrintContentList *prcl= new PrintContentList();
                prcl->act(*this);
                setActiveLog(prcl);
            }
                break;

            case 5:{//PrintWatchHistory
                PrintWatchHistory *printHistory = new PrintWatchHistory();
                printHistory->act(*this);
                setActiveLog(printHistory);

            }
                break;

            case 6:{//Watch
               int id;
               std:: cin>>id;
               Watch *watch = new Watch(id-1);
               watch->act(*this);
            }
                break;

            case 7:{//PrintActionsLog
                PrintActionsLog *pal=new PrintActionsLog();
                pal->act(*this);
                setActiveLog(pal);
            }
                break;

            case 9:{//any illegal input
                std::cout<<"invalid input"<<std::endl;
            }
                break;

        }
    }while (i!=8);//Exit
    Exit *exit=new Exit();
    exit->act(*this);
    setActiveLog(exit);

}


