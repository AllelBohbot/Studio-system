#include "../include/Trainer.h"

//constructor
Trainer::Trainer(int t_capacity):
        capacity(t_capacity), open(false), salary(0), customersList(), orderList() {}

void Trainer::clear() {
    for(size_t i=0; i<customersList.size();i++){
        if (customersList[i] != nullptr) {
            delete customersList[i];
            customersList[i] = nullptr;
        }
    }
    open = false;
    salary = 0;
    customersList.clear();
    orderList.clear();
}

//destructor
Trainer::~Trainer(){
    clear();
}

//copy ctr
Trainer::Trainer(const Trainer& other):capacity(other.capacity),open(other.open),salary(other.salary),customersList(), orderList(){
    for(size_t i=0;i<other.customersList.size();i++){
        if (other.customersList[i] != nullptr) {
            Customer* customer = other.customersList[i]->clone();
            customersList.push_back(customer);
        }
    }
    for (size_t i = 0; i < other.orderList.size(); i++) {
        orderList.push_back(other.orderList[i]);
    }
}

//move ctr
Trainer::Trainer(Trainer&& other): capacity(other.capacity),open(other.open),salary(other.salary),customersList(std::move(other.customersList)), orderList(std::move(other.orderList)){
    other.customersList.clear();
    other.orderList.clear();
    other.capacity=0;
    other.open= false;
    other.salary = 0;
}

//copy assignment operator
Trainer& Trainer::operator=(const Trainer& other){
    if (this != &other) {
        capacity = other.capacity;
        open = other.open;
        salary = other.salary;
        for(size_t i=0;i<other.customersList.size();i++){
            customersList.push_back(other.customersList[i]->clone());
        }
        for(size_t i=0;i<other.orderList.size();i++){
            orderList.push_back(other.orderList[i]);
        }
    }
    return *this;
}

// move assignment
Trainer& Trainer::operator=(Trainer&& other) {
    if (this != &other) {
        clear();
        capacity=other.capacity;
        open = other.open;
        salary = other.salary;
        orderList = std::move(other.orderList);
        customersList = std::move(other.customersList);
        other.customersList.clear();
        other.orderList.clear();
        other.capacity = 0;
        other.open = false;
        other.salary = 0 ;
    }
    return *this;
}

int Trainer::getCapacity() const {
    return capacity;
}

void Trainer::addCustomer(Customer* customer) {
    customersList.push_back(customer);
}


void Trainer:: removeCustomer(int id) {
    std::vector<Customer*>::iterator it = customersList.begin();
    while (it != customersList.end()) {
        if (*it == getCustomer(id))
            it = customersList.erase(it);
        else
            it++;
    }
}

Customer* Trainer::getCustomer(int id) {
    for (size_t i = 0; i < customersList.size() ; ++i) {
        if ((*customersList[i]).getId() == id) {
            return customersList[i];
        }
    }
    return nullptr;
}

std::vector<Customer*>& Trainer::getCustomers() {
    return customersList;
}

std::vector<OrderPair>& Trainer::getOrders() {
    return orderList;
}


void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options) {}
void Trainer::openTrainer(){}

void Trainer::closeTrainer(){
    clear();
}

int Trainer::getSalary(){
    return salary;
}

bool Trainer::isOpen(){
    return open;
}


void Trainer::setOpen(bool check){
    open = check;
}

void Trainer::setSalary(int add) {
    salary = salary + add;
}

void Trainer::deletePair(int id) {
    std::vector<OrderPair> pairs = orderList;
    orderList.clear();
    size_t index = 0;
    for (size_t i = 0; i < pairs.size(); i++) {
        if (pairs[i].first != id) {
            orderList.push_back(pairs[i]);
            if (i <= index)
                index++;
        }
    }
}