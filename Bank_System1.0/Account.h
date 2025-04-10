#pragma once

#include <string>

class Account {
public:
    Account() {};
    Account(const std::string& id, const std::string& customerId,
        const std::string& accountNumber, double balance);

    std::string GetAccountID() const;
    std::string GetCustomerID() const;
    std::string GetAccountNumber() const;
    double GetBalance() const;

    void Deposit(double amount);
    bool Withdraw(double amount);

private:
    std::string accountID;
    std::string customerID;
    std::string accountNumber;
    double balance;
};
