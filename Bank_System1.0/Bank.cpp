#define _CRT_SECURE_NO_WARNINGS
#include "Bank.h"
#include <iostream>
#include <fstream>

Bank::Bank() : db("bank2.db") 
{
    LoadCustomersFromDatabase();
    LoadAccountsFromDatabase();
}

bool Bank::CreateCustomer(const std::string& id, const std::string& firstName, const std::string& lastName,
    int age, int password, const std::string& cnp) {
    if (customers.find(id) != customers.end()) return false;

    Customer newCustomer(id, firstName, lastName, age, password, cnp);
    customers[id] = newCustomer;

    return db.InsertCustomer(newCustomer);
}

bool Bank::RemoveCustomer(const std::string& id) {
    bool removedFromMap = (customers.erase(id) > 0);
    bool removedFromDB = db.DeleteCustomer(id);
    return removedFromMap || removedFromDB; // dacă e șters din cel puțin unul
}

bool Bank::DeleteAccount(const std::string& accountId) {
    bool removedFromMap = (accounts.erase(accountId) > 0);
    bool removedFromDB = db.DeleteAccount(accountId);
    return removedFromMap || removedFromDB;
}



Customer* Bank::LoginCustomer(const std::string& firstName, const std::string& lastName, int password) {
    for (auto& pair : customers) {
        Customer& customer = pair.second;
        if (customer.GetFirstName() == firstName &&
            customer.GetLastName() == lastName &&
            customer.GetPassword() == password) {
            return &customer;
        }
    }

    Customer* loaded = db.LoadCustomer(firstName, lastName, password);
    if (loaded) {
        customers[loaded->GetCustomerID()] = *loaded;
        delete loaded;
        return &customers[loaded->GetCustomerID()];
    }
    return nullptr;
}

void Bank::GetCustomerData(const std::string& customerId,bool toConsole = true, bool toFile = false)
{
    Customer* customer = FindCustomer(customerId);
    if (!customer) {
        if (toConsole) std::cout << "Customer not found.\n";
        return;
    }

    std::ofstream out;
    if (toFile) {
        std::time_t now = std::time(nullptr);
        char dateStr[20];
        std::strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", std::localtime(&now));

        out.open("statement_" + customer->GetFirstName() + "_" + dateStr + ".txt");
        if (!out) {
            if (toConsole) std::cout << "Failed to create file.\n";
            return;
        }
    }

    auto write = [&](const std::string& line) {
        if (toConsole) std::cout << line << "\n";
        if (toFile)    out << line << "\n";
        };

    write("====== CUSTOMER STATEMENT ======");
    write("Customer ID: " + customer->GetCustomerID());
    write("Name: " + customer->GetFirstName() + " " + customer->GetLastName());
    write("Age: " + std::to_string(customer->GetAge()));
    write("CNP: " + customer->GetCNP());
    write("Password: " + std::to_string(customer->GetPassword()));
    write("");

    auto accounts = GetAccounts(customerId);
    if (accounts.empty()) {
        write("This customer has no accounts.");
    }
    else {
        write("--- Accounts ---");
        for (const auto* acc : accounts) {
            write("Account ID: " + acc->GetAccountID() +
                " | Number: " + acc->GetAccountNumber() +
                " | Balance: " + std::to_string(acc->GetBalance()));
        
            write("--- Transactions ---");

            std::vector<Transaction> tx = db.GetTransactionsForAccount(acc->GetAccountID());
            if (tx.empty()) {
                write("   No transactions.");
            }
            else {
                for (const auto& t : tx) {
                    write("   " + t.GetTimestamp() + " | " + t.GetID() +
                        " | " + t.GetType() + " | Amount: " + std::to_string(t.GetAmount()));
                }
            }
            write("");
        }
    }

    write("===============================");
    if (toFile) out.close();

    if (toConsole && toFile) {
        std::cout << "Statement also saved to file: statement_" << customerId << ".txt\n";
    }
    else if (toFile) {
        std::cout << "Statement saved to file: statement_" << customerId << ".txt\n";
    }

    std::cout << "===========================\n";
}


bool Bank::CreateAccount(const std::string& accountId, const std::string& customerId,
    const std::string& accountNumber, double initialBalance) {
    // Check if account ID already exists
    if (accounts.find(accountId) != accounts.end()) {
        std::cout << "Account ID already exists.\n";
        return false;
    }

    // Check if customer exists
    if (customers.find(customerId) == customers.end()) {
        std::cout << "Customer ID not found.\n";
        return false;
    }

    // Create and add the account
    Account newAccount(accountId, customerId, accountNumber, initialBalance);
    accounts[accountId] = newAccount;

    std::cout << "Account created successfully for customer " << customerId << ".\n";
    return db.InsertAccount(newAccount);
}

std::vector<Transaction> Bank::GetAllTransactions()
{
    return db.LoadAllTransactions();

}

void Bank::TransactionId(const std::string& customerId)
{
    std::vector<Account*> accounts= GetAccounts(customerId);
    for (const auto& account : accounts)
    {
        std::vector<Transaction> transactions = db.GetTransactionsForAccount(account->GetAccountID());

        if (transactions.empty())
            std::cout << "\nYou don't have any  transactions\n";

        for (const auto& t : transactions)
        {
            std::cout << t.GetTimestamp() << " | " << t.GetID()
                << " | Account: " << t.GetAccountID()
                << " | " << t.GetType()
                << " | Amount: " << t.GetAmount() << "\n";
        }
    }

}


bool Bank::Withdraw(const std::string& accountId, double amount) {
    auto it = accounts.find(accountId);
    if (it == accounts.end()) return false;

    return it->second.Withdraw(amount);
}

void Bank::ShowAllCustomers() const {
    for (const auto& pair : customers) {
        const Customer& c = pair.second;
        std::cout << c.GetCustomerID() << ": " << c.GetFirstName() << " " << c.GetLastName() << "\n";
    }
}

void Bank::ShowAllAccounts() const {
    for (const auto& pair : accounts) {
        const Account& acc = pair.second;
        std::cout << acc.GetAccountID()<<", "<<acc.GetCustomerID() << ": " << acc.GetAccountNumber() << ", Balance: " << acc.GetBalance() << "\n";
    }
}

Customer* Bank::FindCustomer(const std::string& id) {
    auto it = customers.find(id);
    if (it != customers.end()) {
        return &it->second;
    }
    return nullptr;
}

std::vector<Account*> Bank::GetAccounts(const std::string& customerId)
{
    std::vector<Account*> result;

    for (auto& pair : accounts) {
        Account& acc = pair.second;
        if (acc.GetCustomerID() == customerId) {
            result.push_back(&acc);
        }
    }

    return result;
}

void Bank::LoadCustomersFromDatabase()
{
    std::vector<std::string> ids = db.GetAllCustomerIDs();
    for (const std::string& id : ids) {
        Customer* c = db.LoadCustomerByID(id);
        if (c) {
            customers[c->GetCustomerID()] = *c;
            delete c;
        }
    }
}

void Bank::LoadAccountsFromDatabase()
{
    std::vector<Account> allAccounts = db.LoadAllAccounts();
    for (const Account& acc : allAccounts) {
        accounts[acc.GetAccountID()] = acc;
    }
}

void Bank::DeleteAllCustomer(const std::string& customerId)
{
    Customer* customer = FindCustomer(customerId);
    if (!customer) {
        std::cout << "Customer not found.\n";
        return;
    }

    // Șterge toate conturile clientului
    auto accounts = GetAccounts(customerId);
    for (auto* acc : accounts) {
       DeleteAccount(acc->GetAccountID());
    }
    db.DeleteTransactionsForCustomer(customerId);
    // Șterge clientul
    if (RemoveCustomer(customerId)) {
        std::cout << "Customer and all associated accounts deleted successfully.\n";
    }
    else {
        std::cout << "Failed to delete customer.\n";
    }
}

//Transaction


bool Bank::Deposit(const std::string& accountId, double amount) {
    auto it = accounts.find(accountId);
    if (it == accounts.end()) return false;

    it->second.Deposit(amount);

    // Generate timestamp
    std::time_t now = std::time(nullptr);
    char buf[100];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    // Generate transaction ID (ex: T001, T002...)
    std::string transactionId = "T" + std::to_string(rand() % 10000); // sau un contor real

    Transaction t(transactionId, accountId, "deposit", amount, buf);
    db.InsertTransaction(t);

    return true;
}


