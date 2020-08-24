#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"


class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    void start();

    //rule of 5:
    ~Session();
    Session(const Session &other);//copy constructor
    Session(Session &&other);//move constructor
    Session&operator=(const Session &other);//copy assignment
    Session&operator=(Session &&other);//move assignment
    Session();//default constructor



    User* getActiveUser();
    const std::vector<Watchable*>& getContent() const;
     std::unordered_map<std::string,User*>& getMap() ;
    std:: vector<BaseAction*> getActionLogVec();
    void setActiveLog(BaseAction *action);
    void setActiveUser(User *newUser);

private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;

    void clean();
    void  copy(const Session &other);
};
#endif
