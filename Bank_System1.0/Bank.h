#pragma once

#include <string>
#include <unordered_map>
#include "Customer.h"
#include "Account.h"
#include "Transaction.h"
#include "DatabaseHandler.h"

#include <ctime>
class Bank {
public:
    Bank();

    // Customer management
    bool CreateCustomer(const std::string& id, const std::string& firstName, const std::string& lastName,
        int age, int password, const std::string& cnp);


    Customer* LoginCustomer(const std::string& firstName, const std::string& lastName, int password);
    void GetCustomerData(const std::string& customerId, bool toConsole, bool toFile);

    // Account management
    bool CreateAccount(const std::string& accountId, const std::string& customerId,
        const std::string& accountNumber, double initialBalance);
    bool RemoveCustomer(const std::string& id);
    bool DeleteAccount(const std::string& accountId);


    // Transactions
    bool Withdraw(const std::string& accountId, double amount);

    void ShowAllCustomers() const;
    void ShowAllAccounts() const;
    bool Deposit(const std::string& accountId, double amount);
    std::vector<Transaction> GetAllTransactions();
    void TransactionId(const std::string& customerId);


    Customer* FindCustomer(const std::string& id); // optional helper

    std::vector<Account*> GetAccounts(const std::string& customerId);


    void LoadCustomersFromDatabase();
    void LoadAccountsFromDatabase();
    void DeleteAllCustomer(const std::string& id);



  private:
    std::unordered_map<std::string, Customer> customers;
    std::unordered_map<std::string, Account> accounts;
    std::unordered_map<std::string, Transaction> transactions;
    DatabaseHandler db;
   
};
