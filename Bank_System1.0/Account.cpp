#include "Account.h"

Account::Account(const std::string& id, const std::string& customerId,
    const std::string& accountNumber, double balance)
    : accountID(id), customerID(customerId), accountNumber(accountNumber), balance(balance) {}

std::string Account::GetAccountID() const {
    return accountID;
}

std::string Account::GetCustomerID() const {
    return customerID;
}

std::string Account::GetAccountNumber() const {
    return accountNumber;
}

double Account::GetBalance() const {
    return balance;
}

void Account::Deposit(double amount) {
    balance += amount;
}

bool Account::Withdraw(double amount) {
    if (amount > balance) return false;
    balance -= amount;
    return true;
}
