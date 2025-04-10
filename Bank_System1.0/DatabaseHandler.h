#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>
#include "Customer.h"
#include "Account.h"
#include "Transaction.h"

class DatabaseHandler {
public:
    DatabaseHandler(const std::string& dbName = "bank2.db");
    ~DatabaseHandler();

    void InitTables();

    // Customer methods
    bool InsertCustomer(const Customer& customer);

    //Delete
    bool DeleteCustomer(const std::string& customerId);
    bool DeleteAccount(const std::string& accountId);
    bool DeleteTransactionsForCustomer(const std::string& customerId);


    Customer* LoadCustomer(const std::string& firstName, const std::string& lastName, int password);

    // Account methods
    bool InsertAccount(const Account& account);
    std::vector<Account> LoadAccountsForCustomer(const std::string& customerId);


    std::vector<std::string> GetAllCustomerIDs();
    Customer* LoadCustomerByID(const std::string& customerId);
    std::vector<Account> LoadAllAccounts();
    std::vector<Transaction> LoadAllTransactions();
    Transaction* LoadTransactionByID(const std::string& transactionId);




    //tranzation
    bool InsertTransaction(const Transaction& transaction);
    std::vector<Transaction> GetTransactionsForAccount(const std::string& accountId);

private:
    sqlite3* db;
};
