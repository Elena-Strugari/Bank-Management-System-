#pragma once

#include <string>
#include <vector>


class Customer {
public:
    Customer() {};
    Customer(const std::string& id,const std::string& firstName, const std::string& lastName,
        int age,int password,const std::string& cnp);

    std::string GetCustomerID() const;
    std::string GetFirstName() const;
    std::string GetLastName() const;
    std::string GetCNP() const;
    int GetAge() const;
    int GetPassword() const;

private:
    std::string customerID;
    std::string firstName;
    std::string lastName;
    int age;
    int password;
    std::string cnp;
};
