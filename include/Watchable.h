#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);
    virtual ~Watchable();
    Watchable();
    Watchable(const Watchable &other);//copy constructor
    Watchable& operator=(const Watchable &other);//copy assignment operator

    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session& s) const = 0;
    virtual Watchable* nextWatchableHelper(Session& s, std::string nameLast) =0;
    virtual Watchable* clone()=0;

    virtual const int & getLength() const;
    virtual const std::vector<std::string> & getTags() const;
    virtual const long & getId() const;
private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    Movie(const Movie &other);
    Movie& operator=(const Movie &other);

    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session& s) const;
    virtual Watchable* nextWatchableHelper(Session& s, std::string nameLast) ;

    Movie* clone();

private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    Episode(const Episode &other);
    Episode&operator=(const Episode &other);


    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session& s) const;
    virtual Watchable* nextWatchableHelper(Session& s, std::string nameLast) ;

    Episode* clone();

private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif
