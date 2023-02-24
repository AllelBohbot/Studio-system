#include "../include/Action.h"
using namespace std;
extern Studio* backup;

BaseAction::BaseAction(){}

ActionStatus BaseAction:: getStatus() const{
    return status;
}

void BaseAction::complete(){
    status=COMPLETED;
}

void BaseAction::error(std::string errorMsg){
    this->errorMsg=errorMsg;
    status=ERROR;
}

std::string BaseAction::getErrorMsg() const{
    return errorMsg;
}

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList)
        :BaseAction(), trainerId(id), customers(){
    customers = std::move(customersList);
}


void OpenTrainer::act(Studio &studio) {
    actionMessage = "open " + std::to_string(trainerId) + " ";
    if ((!studio.getTrainer(trainerId)) || (studio.getTrainer(trainerId))->isOpen()) {
        error("Workout session does not exist or is already open");
        std::cout << "Error: " << getErrorMsg() << std::endl;
        actionMessage = actionMessage + "Error: " + getErrorMsg();
        for (auto customer: customers) {
            if (customer != nullptr)  {
                delete customer;
                customer = nullptr;
            }
        }
    }
    else {
        for (size_t i = 0; i < customers.size(); i++) {
            if ((int) studio.getTrainer(trainerId)->getCustomers().size() <
                studio.getTrainer(trainerId)->getCapacity()) {
                (studio.getTrainer(trainerId))->getCustomers().push_back(customers[i]);
                actionMessage = actionMessage + customers[i]->getName() + "," + customers[i]->toString() + " ";
            } else { // trainer capacity has been surpassed
                if (customers[i] != nullptr) {
                    delete customers[i];
                    customers[i] = nullptr;
                }
            }
        }
        studio.getTrainer(trainerId)->setOpen(true);
        complete();
        actionMessage = actionMessage + "Completed";
    }
}

std::string OpenTrainer::toString() const {
    return actionMessage;
}

OpenTrainer* OpenTrainer::clone() {
    return new OpenTrainer(*this);
}

Order::Order(int id): trainerId(id) {}

void Order::act(Studio &studio) {
    actionMessage = "order " + std::to_string(trainerId) + " ";
    if (studio.getTrainer(trainerId) == nullptr || !studio.getTrainer(trainerId)->isOpen()) {
        error("Trainer does not exist or is not open");
        actionMessage = actionMessage + "Error: " + getErrorMsg();
        std::cout << "Error: " << getErrorMsg() << std::endl;
    }
    else {
        for (size_t i = 0; i < studio.getTrainer(trainerId)->getCustomers().size(); i++) {
            std::vector<int> order = (studio.getTrainer(trainerId)->getCustomers()[i])->order(studio.getWorkoutOptions());
            for (size_t j = 0; j < order.size(); j++) {
                studio.getTrainer(trainerId)->getOrders().push_back(
                        std::make_pair(studio.getTrainer(trainerId)->getCustomers()[i]->getId(), studio.getWorkoutOptions()[order[j]]));
                studio.getTrainer(trainerId)->setSalary(studio.getWorkoutOptions()[order[j]].getPrice());
                std::string orderPrint = studio.getTrainer(trainerId)->getCustomers()[i]->getName() + " Is Doing " + studio.getWorkoutOptions()[order[j]].getName();
                std::cout << orderPrint << std::endl;
            }
        }
        complete();
        actionMessage = actionMessage + "Completed";
    }
}

std::string Order::toString() const{
    return actionMessage;
}

Order * Order::clone() {
    return new Order(*this);
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId)
        :srcTrainer(src),dstTrainer(dst),id(customerId){}

void MoveCustomer::act(Studio &studio){
    actionMessage = "move " + std::to_string(srcTrainer) +" "+ std::to_string(dstTrainer) + " " + std::to_string(id);
    if(studio.getTrainer(srcTrainer) == nullptr || studio.getTrainer(dstTrainer) == nullptr
        || !studio.getTrainer(srcTrainer)->isOpen() || !studio.getTrainer(dstTrainer)->isOpen()
        || studio.getTrainer(srcTrainer)->getCustomer(id) == nullptr || studio.getTrainer(dstTrainer)->getCapacity() == (int)studio.getTrainer(dstTrainer)->getCustomers().size()
        ){
        error("Cannot move customer");
        actionMessage = actionMessage + " Error: " + getErrorMsg();
        std::cout << "Error: " << getErrorMsg() << std::endl;
    }
    else {
        Trainer *src = studio.getTrainer(srcTrainer);
        Trainer *dst = studio.getTrainer(dstTrainer);
        dst->addCustomer(src->getCustomer(id));
        src->removeCustomer(id);
        if (!src->getOrders().empty()) {
            for (OrderPair pair : src->getOrders()) {
                if (pair.first == id) {
                    int price = pair.second.getPrice();
                    src->setSalary(-price);
                    dst->setSalary(price);
                    dst->getOrders().push_back(pair);
                    src->deletePair(id);
                }
            }
        }
        if(src!= nullptr) {
            src = nullptr;
        }
        if(dst!= nullptr) {
            dst = nullptr;
        }
        complete();
        actionMessage = actionMessage + " Completed";
    }
}


std::string MoveCustomer::toString() const{
    return actionMessage;
}

MoveCustomer * MoveCustomer::clone() {
    return new MoveCustomer(*this);
}

Close::Close(int id): trainerId(id){}

void Close::act(Studio &studio){
    actionMessage = "close " + std::to_string(trainerId);
    if(!studio.getTrainer(trainerId)) {
        error("Trainer does not exist or is not open");
        actionMessage = actionMessage + " Error: " + getErrorMsg();
        std::cout << "Error: " << getErrorMsg() << std::endl;
    }
    else {
        int salary=studio.getTrainer(trainerId)->getSalary();
        std::cout <<"Trainer "<< std::to_string(trainerId)<<" closed. Salary " <<salary<<"NIS"<< std::endl;
        studio.getTrainer(trainerId)->closeTrainer();
        complete();
        actionMessage = actionMessage + " Completed";
    }
}

std::string Close::toString() const{
    return actionMessage;
}

Close * Close::clone() {
    return new Close(*this);
}

CloseAll::CloseAll(){}

void CloseAll::act(Studio &studio){
    int i = 0;
    for (Trainer* trainer: studio.getTrainers()) {
        if (trainer->isOpen()) {
            std::cout << "Trainer " << std::to_string(i) << " closed. Salary " << std::to_string(trainer->getSalary())
                      << "NIS" << std::endl;
            trainer->closeTrainer();
        }
        i++;
    }
    complete();
    actionMessage = "closeall Completed";
}

std::string CloseAll::toString() const{
    return actionMessage;
}

CloseAll * CloseAll::clone() {
    return new CloseAll(*this);
}

PrintWorkoutOptions::PrintWorkoutOptions(){}

void PrintWorkoutOptions::act(Studio &studio){
    std::vector<Workout>workout = studio.getWorkoutOptions();
    for(size_t i=0;i<workout.size();i++) {
        std::string name = workout[i].getName();
        WorkoutType type = workout[i].getType();
        int price = workout[i].getPrice();
        std::cout << name << ", " <<type << ", " <<price << std::endl;
    }
    complete();
    actionMessage = "workout_options Completed";
}



std::string PrintWorkoutOptions::toString() const{
    return actionMessage;
}

PrintWorkoutOptions * PrintWorkoutOptions::clone() {
    return new PrintWorkoutOptions(*this);
}

PrintTrainerStatus::PrintTrainerStatus(int id)
        :trainerId(id){}

void PrintTrainerStatus::act(Studio &studio){
    if(studio.getTrainer(trainerId)->isOpen()){
        std::cout<<"Trainer "<< std::to_string(trainerId)<<" status: open"<<endl;
        cout<<"Customers:"<<endl;
        vector<Customer*> customers=studio.getTrainer(trainerId)->getCustomers();
        for(size_t i=0;i<customers.size();i++){
            cout<<customers[i]->getId()<<" "<<customers[i]->getName()<<endl;
        }
        cout<<"Orders:"<<endl;
        vector<OrderPair> orders=studio.getTrainer(trainerId)->getOrders();
        for(size_t i=0;i<orders.size();i++){
            string name=orders[i].second.getName();
            int price=orders[i].second.getPrice();
            int customerId=orders[i].first;
            cout<<name<<" "<<price<<"NIS "<<customerId<<endl;
        }
        cout<<"Current Trainer's Salary: "<<studio.getTrainer(trainerId)->getSalary()<<"NIS"<<endl;
    }
    else
        cout<<"Trainer "<< std::to_string(trainerId) <<" status: closed"<<endl;
    complete();
    actionMessage = "status " + std::to_string(trainerId) + " Completed";
}

std::string PrintTrainerStatus::toString() const{
    return actionMessage;
}

PrintTrainerStatus * PrintTrainerStatus::clone() {
    return new PrintTrainerStatus(*this);
}

PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog::act(Studio &studio){
    for (size_t i = 0; i < studio.getActionsLog().size(); i++){
        std::string s = studio.getActionsLog()[i]->toString();
        std::cout << s << std::endl;
    }
    actionMessage = "log Completed";
}

std::string PrintActionsLog::toString() const {
    return actionMessage;
}

PrintActionsLog * PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}

BackupStudio::BackupStudio(): BaseAction(){}

void BackupStudio:: act(Studio &studio){
    if (backup != nullptr) {
        delete backup;
        backup = nullptr;
    }
    backup = new Studio(studio);
    complete();
    actionMessage = "backup Completed";
}

std::string BackupStudio::toString() const {
    return actionMessage;
}

BackupStudio * BackupStudio::clone() {
    return new BackupStudio(*this);
}

RestoreStudio::RestoreStudio() {}

void RestoreStudio::act(Studio &studio) {
    actionMessage = "restore ";
    if (backup == nullptr) {
        error("No backup available");
        actionMessage = actionMessage + "Error: " + getErrorMsg();
        std::cout << "Error: " << getErrorMsg() << std::endl;
    }
    else {
        studio = *backup;
        complete();
        actionMessage = actionMessage + "Completed";
    }
}

std::string RestoreStudio::toString() const {
    return actionMessage;
}

RestoreStudio* RestoreStudio::clone() {
    return new RestoreStudio(*this);
}
