#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Workout.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Workout> &workout_options)=0;      //why returns int vector
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    virtual Customer* clone()=0;
    virtual ~Customer() = default;
private:
    const std::string name;
    const int id;
};


class SweatyCustomer : public Customer {
public:
    SweatyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    SweatyCustomer* clone();
    virtual ~SweatyCustomer() = default;
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    CheapCustomer* clone();
    virtual ~CheapCustomer() = default;


private:
};


class HeavyMuscleCustomer : public Customer {
public:
    HeavyMuscleCustomer(std::string name, int id);
    int maxIdByPrice(std::vector<Workout>& workouts);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    HeavyMuscleCustomer* clone();
    virtual ~HeavyMuscleCustomer() = default;
private:
};


class FullBodyCustomer : public Customer {
public:
    FullBodyCustomer(std::string name, int id);
    int idFinder(std::vector<Workout>& workouts, WorkoutType type);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    FullBodyCustomer* clone();
    virtual ~FullBodyCustomer() = default;
private:
};


#endif