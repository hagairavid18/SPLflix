#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
class Watchable;
class Session;

class User{
public:
    User(const std::string& name);
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;

    virtual void setHistory(Watchable* val,const Session& s);


    virtual User* duplicate(std::string name)=0;
    User();
    User(std::string name,const User &other);//copy constructor
    virtual ~User();//destructor
    User& operator=(const User &other);//copy assignment operator
    User(User&& other);//move constructor
    User& operator=(User &&other);//move assignment operator
    virtual User* clone(const Session &sess) =0;

    void fixHistory(const Session &s);


protected:
    std::vector<Watchable*> history;
    void copyHistory(const User* other);
private:
    const std::string name;

    void clear();

};


class LengthRecommenderUser : public User {
public:
    virtual ~LengthRecommenderUser();
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual LengthRecommenderUser* duplicate(std::string name);

    virtual LengthRecommenderUser* clone(const Session &sess);
private:

};

class RerunRecommenderUser : public User {
public:
    virtual ~RerunRecommenderUser();
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    RerunRecommenderUser* duplicate(std::string name);

    virtual RerunRecommenderUser* clone(const Session &sess);
private:
    int index;
};

//region TagPair

class TagPair{
public:
    TagPair(std::string s);
    ~TagPair();
    std::string getTag();
    int getCounter();
    void setTag(std::string newTag);
    void setCounter(int newCounter);
    void increase();
    int compareTo(TagPair &other);

private:
    std::string tag;
    int counter;
};

//endregion

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    GenreRecommenderUser* duplicate(std::string name);

    virtual ~GenreRecommenderUser();

    virtual void setHistory(Watchable* val,const Session& s);
    void bubble(int index);
    virtual GenreRecommenderUser* clone(const Session &sess);

private:
    std::vector<TagPair> vec_tag;
};


#endif
