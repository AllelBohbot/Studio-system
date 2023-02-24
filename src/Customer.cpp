#include <algorithm>
#include "../include/Customer.h"

Customer::Customer(std::string c_name, int c_id):
        name(c_name),id(c_id){}


std::string Customer::getName() const{
    return name;
}

int Customer::getId() const{
    return id;
}

SweatyCustomer::SweatyCustomer(std::string name, int id):
        Customer(name,id){}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> workout_order;
    for (size_t i = 0; i < workout_options.size() ; ++i) {
        int temp=workout_options[i].getType();
        if(temp==CARDIO)
            workout_order.push_back(workout_options[i].getId());
    }
    return workout_order;
}


std::string SweatyCustomer::toString() const{
    return "swt";
}

SweatyCustomer* SweatyCustomer:: clone(){
    return new SweatyCustomer(*this);
}


CheapCustomer::CheapCustomer(std::string name, int id):
        Customer(name,id){}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options){
    int min=workout_options[0].getPrice();
    int minIndex=0;
    for(size_t i=1;i<workout_options.size();i++){
        if(workout_options[i].getPrice()<min) {
            min = workout_options[i].getPrice();
            minIndex=i;
        }
    }
    std::vector<int> workout_order;
    workout_order.push_back(workout_options[minIndex].getId());
    return workout_order;
}

std::string CheapCustomer::toString() const{
    return "chp";
}

CheapCustomer * CheapCustomer::clone() {
    return new CheapCustomer(*this);
}

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id):
        Customer(name,id){}



// this function returns the id of the workout with the highest price
int HeavyMuscleCustomer::maxIdByPrice(std::vector<Workout>& workouts) {
    int max = 0;
    for (size_t i = 0; i < workouts.size(); i++) {
        if ((workouts[max].getPrice() < workouts[i].getPrice()) & !workouts[i].getOrdered())
            max = i;
    }
    int maxId = workouts[max].getId();
    workouts[max].setOrdered(true);
    return maxId;
}


std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> workout_order;
    std::vector<Workout> anaerobic;
    for (size_t i = 0; i < workout_options.size(); i++) {
        if (workout_options[i].getType() == ANAEROBIC)
            anaerobic.push_back(workout_options[i]);
    }
    for (size_t i = 0; i < anaerobic.size(); ++i) {
        int currMaxId = maxIdByPrice(anaerobic);
        workout_order.push_back(currMaxId);
    }
    return workout_order;
}

std::string HeavyMuscleCustomer:: toString() const{
    return "mcl";
}

HeavyMuscleCustomer * HeavyMuscleCustomer::clone() {
    return new HeavyMuscleCustomer(*this);
}

FullBodyCustomer::FullBodyCustomer(std::string name, int id): Customer(name,id){}


int FullBodyCustomer::idFinder(std::vector<Workout>& workouts, WorkoutType type) {
    if (type == CARDIO || type == ANAEROBIC) {
        int min = 0;
        for (size_t i = 0; i < workouts.size(); i++) {
            if (workouts[i].getPrice() < workouts[min].getPrice())
                min = i;
        }
        return workouts[min].getId();
    }
    else {
            int max = 0;
            for (size_t i = 0; i < workouts.size(); i++) {
                if (workouts[max].getPrice() < workouts[i].getPrice())
                    max = i;
            }
            return workouts[max].getId();
    }
}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> workout_order;
    std::vector<Workout> cardio;
    std::vector<Workout> mixed;
    std::vector<Workout> anaerobic;
    for (size_t i = 0; i < workout_options.size(); i++) {
        if (workout_options[i].getType() == CARDIO) {
            cardio.push_back(workout_options[i]);
        }
        if (workout_options[i].getType() == MIXED) {
            mixed.push_back(workout_options[i]);
        }
        if (workout_options[i].getType() == ANAEROBIC)
            anaerobic.push_back(workout_options[i]);
    }
    if (!cardio.empty()) {
        int cardioId = idFinder(cardio, CARDIO);
        workout_order.push_back(cardioId); }
    if (!mixed.empty()) {
        int mixedId = idFinder(mixed, MIXED);
        workout_order.push_back(mixedId);
    }
    if (!anaerobic.empty()) {
        int anaerobicId = idFinder(anaerobic, ANAEROBIC);
        workout_order.push_back(anaerobicId);
    }
    return workout_order;
}

std::string FullBodyCustomer::toString() const{
    return "fbd";
}

FullBodyCustomer * FullBodyCustomer::clone() {
    return new FullBodyCustomer(*this);
}