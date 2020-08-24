#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>

class Session;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
    BaseAction();
    virtual ~BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Session& sess)=0;
    virtual std::string toString() const=0;

    virtual BaseAction* clone()=0;

protected:
    void complete();
    void error(const std::string& errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;

};

class CreateUser  : public BaseAction {
public:
    CreateUser();
    CreateUser(std::string name, std::string rec); //a new constructor, in order to send the paramaters the user entered
    virtual ~CreateUser();
    virtual void act(Session& sess);
    virtual std::string toString() const;

    CreateUser* clone();


private:
    std::string name; //new field
    std::string rec;//new field
};

class ChangeActiveUser : public BaseAction {
public:
    ChangeActiveUser();
    ChangeActiveUser(std::string name);
    virtual ~ChangeActiveUser();
    virtual void act(Session& sess);
    virtual std::string toString() const;

    ChangeActiveUser* clone();

private:
    std::string name;
};

class DeleteUser : public BaseAction {

public:
    DeleteUser();
    DeleteUser(std::string name);
    virtual ~DeleteUser();
    virtual void act(Session & sess);
    virtual std::string toString() const;

    DeleteUser* clone();


private:
    std::string name;
};


class DuplicateUser : public BaseAction {
public:
    DuplicateUser();
    virtual ~DuplicateUser();
    virtual void act(Session & sess);
    virtual std::string toString() const;
    DuplicateUser(std::string otherName,std::string newName);

    DuplicateUser* clone();

private:
    std::string otherName;
    std::string newName;
};

class PrintContentList : public BaseAction {
public:
    //PrintContentList();//TODO
    virtual ~PrintContentList();
    virtual void act (Session& sess);
    virtual std::string toString() const;

    PrintContentList* clone();
};

class PrintWatchHistory : public BaseAction {
public:
    virtual ~PrintWatchHistory();
    virtual void act (Session& sess);
    virtual std::string toString() const;

    PrintWatchHistory* clone();
};


class Watch : public BaseAction {
public:
    Watch();
    virtual ~Watch();
    Watch(long id);
    virtual void act(Session& sess);
    virtual std::string toString() const;

    Watch* clone();

private:
    long id;
};


class PrintActionsLog : public BaseAction {
public:
    virtual ~PrintActionsLog();
    virtual void act(Session& sess);
    virtual std::string toString() const;

    PrintActionsLog* clone();
};

class Exit : public BaseAction {
public:
    virtual ~Exit();
    virtual void act(Session& sess);
    virtual std::string toString() const;

    Exit* clone();
};
#endif
