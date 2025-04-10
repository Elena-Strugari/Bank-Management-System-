#include "Customer.h"

Customer::Customer(const std::string& id, const std::string& firstName,
    const std::string& lastName, int age,
    int password, const std::string& cnp)
    : customerID(id), firstName(firstName), lastName(lastName),
    age(age), password(password), cnp(cnp) {}

std::string Customer::GetCustomerID() const {
    return customerID;
}

std::string Customer::GetFirstName() const {
    return firstName;
}

std::string Customer::GetLastName() const {
    return lastName;
}

std::string Customer::GetCNP() const {
    return cnp;
}

int Customer::GetAge() const {
    return age;
}

int Customer::GetPassword() const {
    return password;
}

