//
// Created by naglera on 19/11/2019.
//
#include <string>
#include "../include/Watchable.h"
#include "../include/User.h"
#include "../include/Session.h"

//region User

User::User(const std::string &name): history(),name(name){}
User::~User() {}


//User* User::duplicate(std::string name) {}



User& User::operator=(const User &other) {//copy assignment operator
    if(this==(&other))
        return *this;
    clear();
    copyHistory(&other);
    return *this;
}

//getters:
std::vector<Watchable*> User::get_history() const {return history;}
std::string User::getName() const { return name;}

//privet methods:
void User::clear() {
    delete &name;
    for (unsigned int i = 0; i < history.size() ; ++i) {
        history[i]= nullptr;
    }
}


void User::setHistory(Watchable *val,const Session& s) {
    history.push_back(val);
}

void User::copyHistory(const User *other) {
    for (unsigned int i = 0; i <other->history.size() ; ++i) {
        history.push_back(other->history[i]);
    }
}

void User::fixHistory(const Session &s) {
    for (unsigned int i = 0; i <history.size() ; ++i) {
        long id=history[i]->getId();
        history[i]=s.getContent()[id];
    }
}


//endregion

//region LengthRecommenderUser

LengthRecommenderUser::LengthRecommenderUser(const std::string &name):User(name){}
Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    if(history.size()==0)
        return nullptr;
    int avg=0;
    for (unsigned int i = 0; i < history.size() ; ++i) {
        avg+=history[i]->getLength();
    }
    avg=avg/history.size();
    Watchable* ret= nullptr;
    int min_gap=-1;
    bool in_history= false;
    for (unsigned int j = 0; j <s.getContent().size() ; ++j) {
        if((min_gap>abs(s.getContent()[j]->getLength()-avg))|(min_gap==-1)){
            for (unsigned int i = 0; (i <history.size())&!in_history ; ++i) {
                in_history=(history[i]->getId()==s.getContent()[j]->getId());
            }
            if(!in_history) {
                min_gap = abs(s.getContent()[j]->getLength() - avg);
                ret = s.getContent()[j];
            }
            in_history = false;
        }
    }
    return ret;
}

LengthRecommenderUser* LengthRecommenderUser::duplicate(std::string name){
    LengthRecommenderUser *newUser=new LengthRecommenderUser(name);
    newUser->copyHistory(this);
    return newUser;
}

LengthRecommenderUser::~LengthRecommenderUser() {

}
LengthRecommenderUser* LengthRecommenderUser::clone(const Session &sess) {
    LengthRecommenderUser* ret=new LengthRecommenderUser(this->getName());
    ret->copyHistory(this);
    ret->fixHistory(sess);
    return ret;
}

//endregion

//region RerunRecommenderUser

RerunRecommenderUser::RerunRecommenderUser(const std::string &name):User(name),index(0) {}
Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
   index++;
    if(history.size()==0)
        return nullptr;
    return history[index%history.size()];
}
RerunRecommenderUser* RerunRecommenderUser::clone(const Session &sess) {
    RerunRecommenderUser* ret=new RerunRecommenderUser(this->getName());
    ret->copyHistory(this);
    ret->fixHistory(sess);

    ret->index=this->index;
    return ret;
}





RerunRecommenderUser* RerunRecommenderUser::duplicate(std::string name){
    RerunRecommenderUser *newUser=new RerunRecommenderUser(name);
    newUser->copyHistory(this);
    return newUser;
}

RerunRecommenderUser::~RerunRecommenderUser() {

}


//endregion


//region TagPair

TagPair::TagPair(std::string s):tag(s),counter(0) {}

std::string TagPair::getTag() { return tag;}
int TagPair::getCounter() { return counter;}
void TagPair::increase() {counter++;}

int TagPair::compareTo(TagPair &other) {//if this is bigger then other then we will get a possitive number
    if(counter-other.getCounter()!=0)
        return counter-other.getCounter();
    return other.getTag().compare(tag);
}

void TagPair::setCounter(int newCounter) {
    counter=newCounter;
}

void TagPair::setTag(std::string newTag) {
    tag=newTag;
}

TagPair::~TagPair() {}

//endregion

//region GenreRecommenderUser

GenreRecommenderUser::GenreRecommenderUser(const std::string &name):User(name),vec_tag() {}
Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    if (history.size() == 0) {
        return nullptr;
    }
    bool in_history= false;
    bool has_tag= false;
    unsigned int j;
    for (unsigned int i = 0; (i < vec_tag.size())&!has_tag; ++i) {
        for (j = 0; (j < s.getContent().size())&!has_tag; ++j) {
            for (unsigned int k = 0; (k < s.getContent()[j]->getTags().size())&!has_tag; ++k) {//check if watchable has certain tag
                if(((s.getContent()[j]->getTags()[k]).compare(vec_tag[i].getTag()))==0){
                    has_tag= true;
                }
            }
            in_history= false;
            if(has_tag){//if watchable has tag, then check if in history
                for (unsigned int k = 0; (k < history.size())&!in_history; ++k) {
                    in_history=(history[k]->getId()==s.getContent()[j]->getId());
                }
                if(in_history) {//if true, find other watchable with same tag
                    has_tag = false;
                }
            }
        }
    }
    if(has_tag)
        return s.getContent()[j-1];
    return nullptr;
}

GenreRecommenderUser::~GenreRecommenderUser() {}

GenreRecommenderUser* GenreRecommenderUser::duplicate(std::string name) {
    GenreRecommenderUser *newUser = new GenreRecommenderUser(name);
    newUser->copyHistory(this);
    return newUser;
}

void GenreRecommenderUser::setHistory(Watchable *val,const Session& s) {
    history.push_back(val);




    if (vec_tag.size() ==0){
        bool found= false;
        for (unsigned int i = 0; i < s.getContent().size() ; ++i) {
            for (unsigned int j = 0; j < s.getContent()[i]->getTags().size() ; ++j) {
                for (unsigned int k = 0; k < vec_tag.size(); ++k) {
                    if (s.getContent()[i]->getTags()[j]==vec_tag[k].getTag() ) {
                        found = true;
                    }

                }
                if (!found){
                    TagPair newTag=TagPair(s.getContent()[i]->getTags()[j]);
                    vec_tag.push_back(newTag);
                    bubble(vec_tag.size()-1);
                }
            }
        }
    }





    bool found= false;
    for (unsigned int i = 0; i <val->getTags().size() ; ++i) {
        for (unsigned int j = 0; (j < vec_tag.size())&!found; ++j) {
            if((val->getTags()[i].compare(vec_tag[j].getTag()))==0){
                vec_tag[j].increase();
                found= true;
                bubble(j);
            }
        }
        if(!found){
            TagPair newTag=TagPair(val->getTags()[i]);
            vec_tag.push_back(newTag);
            bubble(vec_tag.size()-1);
        }
        found= false;
    }
}

void GenreRecommenderUser::bubble(int index) {
    while(index>0&&(vec_tag[index].compareTo(vec_tag[index-1]))>0){//swap until the vector is sorted
        TagPair temp=TagPair("");

        temp.setCounter(vec_tag[index-1].getCounter());
        temp.setTag(vec_tag[index-1].getTag());

        vec_tag[index-1].setTag(vec_tag[index].getTag());
        vec_tag[index-1].setCounter(vec_tag[index].getCounter());

        vec_tag[index].setTag(temp.getTag());
        vec_tag[index].setCounter(temp.getCounter());

        index--;
    }

}
GenreRecommenderUser* GenreRecommenderUser::clone(const Session &sess) {
    GenreRecommenderUser* ret=new GenreRecommenderUser(this->getName());
    ret->copyHistory(this);
    ret->fixHistory(sess);

    for (unsigned int i = 0; i <this->vec_tag.size() ; ++i) {
        ret->vec_tag.emplace_back(this->vec_tag[i].getTag());
        ret->vec_tag[i].setCounter(this->vec_tag[i].getCounter());
    }
    return ret;
}

//endregion

