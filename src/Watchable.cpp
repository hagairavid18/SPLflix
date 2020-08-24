
//
// Created by naglera on 21/11/2019.
//
#include "../include/Watchable.h"
#include "../include/Session.h"
#include "../include/User.h"
//region Watchable

//constructor:
Watchable::Watchable(long id, int length, const std::vector<std::string> &tags):id(id),length(length),tags(tags){}
//TODO check about how tags work? do i need to deep copy them?
//destructor:
Watchable::~Watchable(){
    tags.clear();
}
//default constructor:
Watchable::Watchable():id(0),length(0),tags() {}
//copy constructor:
Watchable::Watchable(const Watchable &other):id(other.id),length(other.length),tags() {
    for (unsigned int i = 0; i <other.tags.size() ; ++i) {
        tags.push_back(other.tags[i]);
    }
}
//copy assignment operator://TODO: what about id?
Watchable& Watchable::operator=(const Watchable &other) {
    if(this!=&other){
        length=other.length;
        tags=other.tags;
    }
    return *this;
}

const std::vector<std::string> & Watchable::getTags() const { return tags;}
const long & Watchable::getId() const { return id;}
const int & Watchable::getLength() const { return length;}



//endregion

//region Movie

//constructor:
Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags): Watchable(id,length,tags),name(name){}
//destructor:
//copy constructor:
Movie::Movie(const Movie &other): Watchable(other),name(other.name){}
//copy assignment operator:
Movie& Movie::operator=(const Movie &other) {//TODO how to use father method
    name=other.name;
    return *this;
}

std::string Movie::toString() const {
    std::string str=name;
    return str;
}
Watchable* Movie::getNextWatchable(Session &s) const {
    return (s.getActiveUser()->getRecommendation(s));
}
Watchable* Movie::nextWatchableHelper(Session &s, std::string nameLast)  {
    return (s.getActiveUser()->getRecommendation(s));
}

Movie* Movie::clone() {
    Movie *ret=new Movie(*this);
    return ret;
}


//endregion

//region Episode



Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags):Watchable(id,length, tags),seriesName(seriesName),season(season),episode(episode),nextEpisodeId(id+1) {}
Episode::Episode(const Episode &other): Watchable(other),seriesName(other.seriesName),season(other.season),episode(other.episode),
                                        nextEpisodeId(other.nextEpisodeId){}
Episode& Episode::operator=(const Episode &other) {
    Watchable::operator=(other);
    season=other.season;
    episode=other.episode;
    seriesName=other.seriesName;
    nextEpisodeId=other.nextEpisodeId;
    return *this;
}

std::string Episode::toString() const {
    std::string str=seriesName+" S"+std::to_string(season)+"E"+std::to_string(episode);
    return str;
}



Watchable* Episode::getNextWatchable(Session &s) const {
    if((unsigned int)getId()==s.getContent().size()-1)//there is no more cells in content
        return (s.getActiveUser()->getRecommendation(s));
    return (s.getContent()[nextEpisodeId])->nextWatchableHelper(s,seriesName);
    //nextWatchableHelper((s.getContent()[nextEpisodeId]),s);//there is more content but we are not sure if its a movie or an episode
}
Watchable* Episode::nextWatchableHelper(Session &s, std::string nameLast)  {
    if(seriesName.compare(nameLast)==0)//the next watchable in content is an episode from the same series
        return (this);
    return (s.getActiveUser()->getRecommendation(s));
}

Episode* Episode::clone() {
    Episode *ret=new Episode(*this);
    return ret;
}

//endregion


