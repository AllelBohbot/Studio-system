#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"
#include<iostream>
#include<sstream>
#include<fstream>
class BaseAction;


class Studio{
public:
    Studio();
    Studio(const std::string &configFilePath);

    void clear();
    void InputToStudio(std::vector<std::string> &input);
    void start();
    virtual ~Studio();
    Studio(const Studio& other);
    Studio(Studio&& other);
    Studio& operator=(const Studio& other);
    Studio& operator=(Studio&& other);
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    std::vector<Trainer*>& getTrainers();
    void closeBool();
    bool getStatus();
    void doOpen(std::string input);
    void doOrder(std::string input);
    void doMove(std::string input);
    void doClose(std::string input);
    void doCloseAll();
    void doWorkoutOptions();
    void doStatus(std::string input);
    void doLog();
    void doBackup();
    void doRestore();

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    int customerId;
};

#endif