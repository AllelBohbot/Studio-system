#include "../include/Studio.h"

Studio::Studio():customerId(0){}

Studio::Studio(const std::string &configFilePath): Studio(){
    std::ifstream file(configFilePath);
    char line[256];
    std::vector<std::string> input;
    while(file){
        file.getline(line,256);
        if(line[0]=='#') {
            input.push_back("#");
            continue;
        }
        if(line[0]=='\0')
            continue;
        input.push_back(line);
    }
    InputToStudio(input);
}

void Studio::clear() {
    for(size_t i=0;i<trainers.size();i++){
        if(trainers[i]!= nullptr){
            delete trainers[i];
            trainers[i]= nullptr;
        }
    }

    for(size_t i=0;i<actionsLog.size();i++){
        if(actionsLog[i]!= nullptr){
            delete actionsLog[i];
            actionsLog[i]= nullptr;
        }
    }

    workout_options.clear();
    trainers.clear();
    actionsLog.clear();
}

Studio::~Studio(){
    clear();
}

//copy ctr
Studio::Studio(const Studio& other):open(other.open), workout_options(other.workout_options), customerId(other.customerId){
    for(size_t i=0;i<other.trainers.size();i++){
        Trainer* trainer = new Trainer(*(other.trainers[i]));
        trainers.push_back(trainer);
    }
    for(size_t i=0;i<other.actionsLog.size();i++){
        actionsLog.push_back(other.actionsLog[i]->clone());
    }
}

//move ctr
Studio::Studio(Studio&& other): open(other.open), trainers(std::move(other.trainers)), workout_options(std::move(other.workout_options)), actionsLog(std::move(other.actionsLog)), customerId(other.customerId){
    other.trainers.clear();
    other.actionsLog.clear();
    other.workout_options.clear();
    other.open = false;
    other.customerId = 0;
}

//copy assignment operator
Studio& Studio::operator=(const Studio& other) {
    if (this != &other) {
        clear();
        open = other.open;
        customerId = other.customerId;
        for (size_t i = 0; i < other.trainers.size(); i++) {
            Trainer* trainer = new Trainer(*(other.trainers[i]));
            trainers.push_back(trainer);
        }
        for (size_t i = 0; i < other.actionsLog.size(); i++) {
            BaseAction* action = other.actionsLog[i]->clone();
            actionsLog.push_back(action);
        }
        for (size_t i = 0; i < other.workout_options.size(); i++) {
            workout_options.push_back(other.workout_options[i]);
        }
    }
    return *this;
}

//move assignment operator
Studio& Studio::operator=(Studio&& other) {
    if (this != &other) {
        clear();
        open = other.open;
        customerId = other.customerId;
        trainers = std::move(other.trainers);
        actionsLog = std::move(other.actionsLog);
        workout_options = std::move(other.workout_options);
        trainers.clear();
        actionsLog.clear();
        workout_options.clear();
        other.open = false;
        other.customerId = 0;
    }
    return *this;
}


void Studio::InputToStudio(std::vector<std::string> &input){
    int workoutId=0;
    int inputType=0;
    int comaInd = 0;
    for(size_t i=0;i<input.size();i++){
        std::string curr = input[i];
        if(input[i]=="#")
            inputType++;
        else if (inputType == 1) {}
        else if (inputType == 2) {
            while (curr.size() != 0){
                if(curr.at(0) != ','){
                    comaInd = curr.find_first_of(',');
                    int temp = std::stoi(curr.substr(0, comaInd));
                        trainers.push_back(new Trainer(temp));
                    }
                if (comaInd != -1)
                    curr = curr.substr(comaInd + 1, curr.size());
                else
                    curr = "";
                }
        }
        else {
            comaInd = curr.find_first_of(',');
            std::string name = curr.substr(0, comaInd);
            curr = curr.substr(comaInd + 2, curr.size());
            comaInd = curr.find_first_of(',');
            WorkoutType type;
            if (curr.substr(0, comaInd) == "Anaerobic")
                type = ANAEROBIC;
            if (curr.substr(0, comaInd) == "Mixed")
                type = MIXED;
            if (curr.substr(0, comaInd) == "Cardio")
                type = CARDIO;
            curr = curr.substr(comaInd + 2, curr.size());
            int price = std::stoi(curr);
            Workout workout = Workout(workoutId, name, price, type);
            workout_options.push_back(workout);
            workoutId++;
        }
    }
}

void Studio::start(){
    open=true;
    std::cout<<"Studio is now open!"<<std::endl;
    std::string space=" ";
    int pos=0;
    std::string input;
    while (open){
        std::string action;
        std::getline(std::cin,action);
        action = action + " ";
        pos=action.find(space);
        input=action.substr(0,pos);
        action = action.substr(pos+ space.length(), action.size());
        if(input=="open")
            doOpen(action);
        else if(input=="order")
            doOrder(action);
        else if(input=="move")
            doMove(action);
        else if (input=="close")
            doClose(action);
        else if(input=="closeall")
            doCloseAll();
        else if(input=="workout_options")
            doWorkoutOptions();
        else if(input=="status")
            doStatus(action);
        else if(input=="log")
            doLog();
        else if(input=="backup")
            doBackup();
        else if(input=="restore")
            doRestore();
    }
}

int Studio::getNumOfTrainers() const{
    return trainers.size();
}

Trainer* Studio::getTrainer(int tid){
    if(tid >= (int)trainers.size())
        return nullptr;
    return trainers[tid];
}

const std::vector<BaseAction*>& Studio::getActionsLog() const{
    return actionsLog;
}

std::vector<Workout>& Studio::getWorkoutOptions(){
    return workout_options;
}

std::vector<Trainer*>& Studio::getTrainers() {
    return trainers;
}

void Studio::closeBool() {
    open= false;
}

bool Studio::getStatus() {
    return open;
}

void Studio::doOpen(std::string input){
    std::vector<Customer*> customers;
    std::string space=" ";
    int pos=input.find(space);
    std::string name;
    std::string type;
    int trainerId=std::stoi(input.substr(0,pos));
    input=input.erase(0,pos+space.length());
    while (input.size()>0){
        pos=input.find(",");
        name=input.substr(0,pos);
        input.erase(0,pos+1);
        pos=input.find(space);
        type=input.substr(0,pos);
        input.erase(0,pos+space.length());
        if (type == "swt") {
            Customer* customer = new SweatyCustomer(name,customerId);
            customers.push_back(customer);
        }
        else if (type == "chp") {
            Customer* customer = new CheapCustomer(name,customerId);
            customers.push_back(customer);
        }
        else if (type == "mcl") {
            Customer* customer = new HeavyMuscleCustomer(name,customerId);
            customers.push_back(customer);
        }
        else if (type == "fbd") {
            Customer* customer = new FullBodyCustomer(name,customerId);
            customers.push_back(customer);
        }
        customerId++;
    }
    OpenTrainer* action = new OpenTrainer(trainerId, customers);
    action->act(*this);

    customers.clear();
    actionsLog.push_back(action);
}

void Studio::doOrder(std::string input) {
    int id = std::stoi(input);
    Order* action = new Order(id);
    action->act(*this);
    actionsLog.push_back(action);
}

void Studio::doMove(std::string input) {
    int src, dst, customerId;
    std::string space = " ";
    int pos = input.find(space);
    src = std::stoi(input.substr(0,pos));
    input = input.erase(0,pos+space.length());
    pos = input.find(space);
    dst = std::stoi(input.substr(0,pos));
    input = input.erase(0, pos+space.length());
    customerId = std::stoi(input);

    bool srcWasOpen;
    if (getTrainer(src) != nullptr) {
        srcWasOpen = (int)getTrainer(src)->isOpen();
    }

    MoveCustomer* action = new MoveCustomer(src, dst, customerId);
    action->act(*this);
    actionsLog.push_back(action);

    if (getTrainer(src) != nullptr && srcWasOpen && (int) getTrainer(src)->getCustomers().size() == 0) {
            doClose(std::to_string(src));
    }
}

void Studio::doClose(std::string input) {
    int id = std::stoi(input);
    Close* action = new Close(id);
    action->act(*this);
    actionsLog.push_back(action);
}

void Studio::doCloseAll() {
    CloseAll* action = new CloseAll();
    actionsLog.push_back(action);
    action->act(*this);
    clear();
    open= false;
}

void Studio::doWorkoutOptions() {
    PrintWorkoutOptions* action = new PrintWorkoutOptions();
    action->act(*this);
    actionsLog.push_back(action);
}

void Studio::doStatus(std::string input) {
    int id = std::stoi(input);
    PrintTrainerStatus* action = new PrintTrainerStatus(id);
    action->act(*this);
    actionsLog.push_back(action);
}

void Studio::doLog() {
    PrintActionsLog* action = new PrintActionsLog();
    action->act(*this);
    actionsLog.push_back(action);
}

void Studio::doBackup() {
    BackupStudio* action = new BackupStudio();
    action->act(*this);
    actionsLog.push_back(action);
}

void Studio::doRestore() {
    RestoreStudio* action = new RestoreStudio();
    action->act(*this);
    actionsLog.push_back(action);
}