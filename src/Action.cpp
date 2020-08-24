//
// Created by hagairav on 22/11/2019.
//
#include "../include/Session.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include "../include/Watchable.h"
#include "../include/User.h"
#include "../include/Action.h"
using namespace std;

//region BaseAction

BaseAction::BaseAction():errorMsg(),status(PENDING) {}
ActionStatus BaseAction::getStatus() const { return status;}
void BaseAction::complete() {this->status=COMPLETED;}
std::string BaseAction::getErrorMsg() const {return errorMsg;}
void BaseAction::error(const std::string &errorMsg) {
    this->status = ERROR;
    this->errorMsg = errorMsg;
}

BaseAction::~BaseAction() {}

//endregion

//region CreateUser


//-----------------------create User---------------------------
CreateUser::CreateUser():BaseAction(),name(),rec(){}
CreateUser::CreateUser(std::string name, std::string rec):BaseAction(),name(name), rec(std::move(rec)) {}

void CreateUser::act(Session &sess) {

    if (sess.getMap().find(name)!=sess.getMap().end()) {  //if name is already in the map
        error("User with the name " + name + " is already exist");

        //TODO in vase of failure how do i change the status?
    } else if (rec == "len") {  //create an new LengthRecommenderUser
        LengthRecommenderUser *user = new LengthRecommenderUser(name);
        sess.getMap()[name] = user;
        complete();
    } else if (rec == "rer"){
        auto *user = new RerunRecommenderUser(name);
        sess.getMap()[name] = user;
        complete();
    } else if (rec =="gen"){
        GenreRecommenderUser *user = new GenreRecommenderUser(name);
        sess.getMap()[name] = user;
        complete();
    } else{ //in case the user has entered invalid recommender
        error(rec + " recommernder is invalid");
    }
    if (this->getStatus()==ERROR)     std::cout << "ERROR - " + this->getErrorMsg() << std::endl; //in case of an ERROR, print msg

}

std::string CreateUser::toString() const {
    std::string msg;
    if(getStatus()==ERROR) {
        msg += "ERROR :"+getErrorMsg();
    } else if(getStatus()==PENDING) {
        msg += "PENDING";
    } else{
        msg += "COMPLETED";
    }
    std::string str="CreateUser "+msg;
    return str;
}

CreateUser* CreateUser::clone() {
    auto* ret=new CreateUser(this->name,this->rec);
    if(this->getStatus()==COMPLETED)
        ret->complete();
    else if(this->getStatus()==ERROR){
        ret->error(getErrorMsg());
    }
    return ret;
}

CreateUser::~CreateUser() {
}



//endregion

//region changeActiveUser

//----------------------changeActiveUser---------------------

//constructors:
ChangeActiveUser::ChangeActiveUser():BaseAction(),name() {}
ChangeActiveUser::ChangeActiveUser(std::string name):BaseAction(), name(std::move(name))  {}

//public methode
void ChangeActiveUser::act(Session &sess) {

    if (sess.getMap().find(name) == sess.getMap().end()) {  //if name is not in the map
        error(name + " user was never created");
        std::cout << "Error "+ getErrorMsg() << std::endl;
    }
    else { //TODO do i need to add a setter for active user field in session?
        std::unordered_map<std::string, User *>::iterator it; //opens an iterator of the place with "name"
        it = (sess.getMap().find(name)); //assain the iterator

        sess.setActiveUser(it.operator->()->second);//update the active user in session
        complete();
    }
}

std::string ChangeActiveUser::toString() const {
    std::string msg;
    if (getStatus() == ERROR) {
        msg += "ERROR :" + getErrorMsg();
    } else if (getStatus() == PENDING) {
        msg += "PENDING";
    } else {
        msg += "COMPLETED";
    }
    std::string str = "ChangeActiveUser " + msg;
    return str;
}

ChangeActiveUser* ChangeActiveUser::clone() {
    ChangeActiveUser* ret=new ChangeActiveUser(this->name);
    if(this->getStatus()==COMPLETED)
        ret->complete();
    else if(this->getStatus()==ERROR){
        ret->error(getErrorMsg());
    }
    return ret;
}

ChangeActiveUser::~ChangeActiveUser() {}

//endregion

//region DeleteUser

//----------------------deleteActiveUser---------------------
//constructors:
DeleteUser::DeleteUser():BaseAction(),name() {}
DeleteUser::DeleteUser(std::string name): name(std::move(name)) {}

//public nethode
void DeleteUser::act(Session &sess) {
    if (sess.getMap().find(name) == sess.getMap().end()) {  //if name is not in the map
        error(name + " user was never created");
        std::cout << "Error "+ getErrorMsg() << std::endl;

    } else{
        std::unordered_map<std::string, User *>::iterator it; //opens an iterator of the place with "name"
        it = (sess.getMap().find(name)); //assain the iterator
        delete it.operator->()->second; //TODO implement user distructor
        sess.getMap().erase(name); //delete pair from map
        complete();
    }
}

std::string DeleteUser::toString() const {
    std::string msg;
    if (getStatus() == ERROR) {
        msg += "ERROR :" + getErrorMsg();
    } else if (getStatus() == PENDING) {
        msg += "PENDING";
    } else {
        msg += "COMPLETED";
    }
    std::string str = "DeleteUser " + msg;
    return str;
}

DeleteUser* DeleteUser::clone() {
    auto* ret=new DeleteUser(this->name);
    if(this->getStatus()==COMPLETED)
        ret->complete();
    else if(this->getStatus()==ERROR){
        ret->error(getErrorMsg());
    }
    return ret;
}

DeleteUser::~DeleteUser() {}

//endregion

//region DuplicateUser

DuplicateUser::DuplicateUser(std::string otherName, std::string newName):BaseAction(), otherName(std::move(otherName)),newName(std::move(newName)) {}
DuplicateUser::DuplicateUser():BaseAction(),otherName(),newName() {}

void DuplicateUser::act(Session &sess) {
    if (sess.getMap().find(newName)!=sess.getMap().end()) {  //if name is already in the map
        error("User with the name " + newName + " is already exist");
        std::cout << "Error "+ getErrorMsg() << std::endl;
    }
    else if (sess.getMap().find(otherName) == sess.getMap().end()) {  //if name is not in the map
        error(otherName + " user was never created");
        std::cout << "Error "+ getErrorMsg() << std::endl;
    }
    else{
        std::unordered_map<std::string, User *>::iterator it; //opens an iterator of the place with "name"
        it = (sess.getMap().find(otherName)); //assain the iterator

        User *newUser=it.operator->()->second->duplicate(newName);//duplicate other

        sess.getMap()[newName]=newUser;
        complete();
    }
}
std::string DuplicateUser::toString() const {
    std::string msg;
    if (getStatus() == ERROR) {
        msg += "ERROR :" + getErrorMsg();
    } else if (getStatus() == PENDING) {
        msg += "PENDING";
    } else {
        msg += "COMPLETED";
    }
    std::string str = "DuplicateUser " + msg;
    return str;
}

DuplicateUser* DuplicateUser::clone() {
    auto* ret=new DuplicateUser(this->otherName,this->newName);
    if(this->getStatus()==COMPLETED)
        ret->complete();
    else if(this->getStatus()==ERROR){
        ret->error(getErrorMsg());
    }
    return ret;
}

DuplicateUser::~DuplicateUser() {}

//endregion

//region PrintContentList

void PrintContentList::act(Session &sess) {
    for (unsigned int i = 0; i <sess.getContent().size() ; ++i) {
        std::string s_tags="[";
        for (unsigned int j = 0; j <sess.getContent()[i]->getTags().size(); ++j) {
            s_tags+=sess.getContent()[i]->getTags()[j]+", ";
        }
        s_tags.erase(s_tags.size()-2);
        s_tags+="]";
        std::cout<< std::to_string(i+1)+". "+ sess.getContent()[i]->toString()+" "
        +std::to_string(sess.getContent()[i]->getLength())+ " minutes "+s_tags +"\n"<< std::endl;
    }
}
std::string PrintContentList::toString() const {
    std::string msg;
    if (getStatus() == ERROR) {
        msg += "ERROR :" + getErrorMsg();
    } else if (getStatus() == PENDING) {
        msg += "PENDING";
    } else {
        msg += "COMPLETED";
    }
    std::string str = "PrintContentList " + msg;
    return str;
}

PrintContentList* PrintContentList::clone() {
    auto* ret=new PrintContentList();
    if(this->getStatus()==COMPLETED)
        ret->complete();
    else if(this->getStatus()==ERROR){
        ret->error(getErrorMsg());
    }
    return ret;
}

PrintContentList::~PrintContentList(){}


//endregion

//region PrintWatchHistory

void PrintWatchHistory::act(Session &sess) {
    std::cout<< "Watch history for "+sess.getActiveUser()->getName() <<std::endl;
    for (unsigned int i = 0; i < sess.getActiveUser()->get_history().size(); ++i) {
        std::cout<< std::to_string(i+1) + "."+ sess.getActiveUser()->get_history()[i]->toString() <<std::endl;
    }
    complete();
}
std::string PrintWatchHistory::toString() const {
    std::string msg;
    if (getStatus() == ERROR) {
        msg += "ERROR :" + getErrorMsg();
    } else if (getStatus() == PENDING) {
        msg += "PENDING";
    } else {
        msg += "COMPLETED";
    }
    std::string str = "PrintHistoryWatch " + msg;
    return str;
}

PrintWatchHistory* PrintWatchHistory::clone() {
    auto* ret=new PrintWatchHistory();
    if(this->getStatus()==COMPLETED)
        ret->complete();
    else if(this->getStatus()==ERROR){
        ret->error(getErrorMsg());
    }
    return ret;
}

PrintWatchHistory::~PrintWatchHistory() {}

//endregion

//region Watch

Watch::Watch():BaseAction(),id(){}
Watch::Watch(long id):id(id) {}

void Watch::act(Session &sess) {
    if ((unsigned int)id > sess.getContent().size())
        error("no content match this id");
    else {
        std::cout << "Watching " + sess.getContent()[id]->toString() << endl;
        sess.getActiveUser()->setHistory(sess.getContent()[id], sess);

        sess.setActiveLog(this);
        complete();


        Watchable *recomended = sess.getContent()[id]->getNextWatchable(sess);
        if (recomended != nullptr) {
            std::cout << "We recommend watching " + recomended->toString() + " continue watching [y/n]" << endl;


            std::string answer;
            std::cin >> answer;

            if (answer == "y") {
                auto *watch = new Watch(recomended->getId());
                watch->act(sess);
            } else if (answer != "n")
                std::cout << "invalid input" << endl;
        } else
            std::cout << "No content fits the recommendation criteria" << endl;

    }
}
    std::string Watch::toString() const {
        std::string msg;
        if (getStatus() == ERROR) {
            msg += "ERROR :" + getErrorMsg();
        } else if (getStatus() == PENDING) {
            msg += "PENDING";
        } else {
            msg += "COMPLETED";
        }
        std::string str = "Watch " + msg;
        return str;
    }

    Watch* Watch::clone() {
        auto *ret = new Watch(this->id);
        if (this->getStatus() == COMPLETED)
            ret->complete();
        else if (this->getStatus() == ERROR) {
            ret->error(getErrorMsg());
        }
        return ret;
    }

Watch::~Watch() {}



//endregion

//region PrintActionsLog

void PrintActionsLog::act(Session &sess) {
    for (int i = sess.getActionLogVec().size()-1; i >=0; --i) {
        std::cout<< sess.getActionLogVec()[i]->toString() <<std::endl;
    }
    complete();
}
std::string PrintActionsLog::toString() const {
    std::string msg;
    if (getStatus() == ERROR) {
        msg += "ERROR :" + getErrorMsg();
    } else if (getStatus() == PENDING) {
        msg += "PENDING";
    } else {
        msg += "COMPLETED";
    }
    std::string str = "PrintActionLog " + msg;
    return str;
}

PrintActionsLog* PrintActionsLog::clone() {
    auto *ret = new PrintActionsLog();
    if (this->getStatus() == COMPLETED)
        ret->complete();
    else if (this->getStatus() == ERROR) {
        ret->error(getErrorMsg());
    }
    return ret;
}

PrintActionsLog::~PrintActionsLog() {}

//endregion

//region Exit

void Exit::act(Session &sess) {
    complete();
}

std::string Exit::toString() const {
    std::string msg;
    if (getStatus() == ERROR) {
        msg += "ERROR :" + getErrorMsg();
    } else if (getStatus() == PENDING) {
        msg += "PENDING";
    } else {
        msg += "COMPLETED";
    }
    std::string str = "Exit " + msg;
    return str;
}

Exit* Exit::clone() {
    Exit *ret = new Exit();
    if (this->getStatus() == COMPLETED)
        ret->complete();
    else if (this->getStatus() == ERROR) {
        ret->error(getErrorMsg());
    }
    return ret;
}

Exit::~Exit() {}

//endregion

