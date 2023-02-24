#ifndef ACTION_H_
#define ACTION_H_
#include <string>
#include <iostream>
#include "Customer.h"
#include "Studio.h"


enum ActionStatus{
    COMPLETED, ERROR
};

//Forward declaration
class Studio;

class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Studio& studio)=0;
    virtual std::string toString() const=0;
    virtual BaseAction* clone()=0;
    virtual ~BaseAction() = default;
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer *> &customersList);
    void act(Studio &studio);
    std::string toString() const;
    OpenTrainer* clone();
    virtual ~OpenTrainer() = default;
private:
    const int trainerId;
    std::vector<Customer *> customers;
    std::string actionMessage;
};

class Order : public BaseAction {
public:
    Order(int id);
    void act(Studio &studio);
    std::string toString() const;
    Order* clone();
    virtual ~Order() = default;
private:
    const int trainerId;
    std::string actionMessage;
};

class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Studio &studio);
    std::string toString() const;
    MoveCustomer* clone();
    virtual ~MoveCustomer() = default;
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
    std::string actionMessage;
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Studio &studio);
    std::string toString() const;
    Close* clone();
    virtual ~Close() = default;
private:
    const int trainerId;
    std::string actionMessage;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Studio &studio);
    std::string toString() const;
    CloseAll* clone();
    virtual ~CloseAll() = default;
private:
    std::string actionMessage;
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    void act(Studio &studio);
    std::string toString() const;
    PrintWorkoutOptions* clone();
    virtual ~PrintWorkoutOptions() = default;
private:
    std::string actionMessage;
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    void act(Studio &studio);
    std::string toString() const;
    PrintTrainerStatus* clone();
    virtual ~PrintTrainerStatus() = default;
private:
    const int trainerId;
    std::string actionMessage;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Studio &studio);
    std::string toString() const;
    PrintActionsLog* clone();
    virtual ~PrintActionsLog() = default;
private:
    std::string actionMessage;
};

class BackupStudio : public BaseAction {
public:
    BackupStudio();
    void act(Studio &studio);
    std::string toString() const;
    BackupStudio* clone();
    virtual ~BackupStudio() = default;
private:
    std::string actionMessage;
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    void act(Studio &studio);
    std::string toString() const;
    RestoreStudio* clone();
    virtual ~RestoreStudio() = default;
private:
    std::string actionMessage;
};


#endif