#include "DatabaseHandler.h"
#include <iostream>

DatabaseHandler::DatabaseHandler(const std::string& dbName) {
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database\n";
    }
    else {
        InitTables();
    }
}

DatabaseHandler::~DatabaseHandler() {
    sqlite3_close(db);
}

void DatabaseHandler::InitTables() {
    const char* customersQuery =
        "CREATE TABLE IF NOT EXISTS customers ("
        "customer_id TEXT PRIMARY KEY, "
        "first_name TEXT, "
        "last_name TEXT, "
        "age INTEGER, "
        "password INTEGER, "
        "cnp TEXT UNIQUE);";

    const char* accountsQuery =
        "CREATE TABLE IF NOT EXISTS accounts ("
        "account_id TEXT PRIMARY KEY, "
        "customer_id TEXT, "
        "account_number TEXT, "
        "balance REAL, "
        "FOREIGN KEY(customer_id) REFERENCES customers(customer_id));";

    const char* transactionsQuery =
        "CREATE TABLE IF NOT EXISTS transactions ("
        "transaction_id TEXT PRIMARY KEY, "
        "account_id TEXT, "
        "type TEXT, "
        "amount REAL, "
        "timestamp TEXT, "
        "FOREIGN KEY(account_id) REFERENCES accounts(account_id));";

    sqlite3_exec(db, customersQuery, nullptr, nullptr, nullptr);
    sqlite3_exec(db, accountsQuery, nullptr, nullptr, nullptr);
    sqlite3_exec(db, transactionsQuery, nullptr, nullptr, nullptr);
}

bool DatabaseHandler::InsertCustomer(const Customer& customer) {
    std::string query = "INSERT INTO customers (customer_id, first_name, last_name, age, password, cnp) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    

    sqlite3_bind_text(stmt, 1, customer.GetCustomerID().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, customer.GetFirstName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, customer.GetLastName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, customer.GetAge());
    sqlite3_bind_int(stmt, 5, customer.GetPassword());
    sqlite3_bind_text(stmt, 6, customer.GetCNP().c_str(), -1, SQLITE_TRANSIENT);


    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}
bool DatabaseHandler::DeleteCustomer(const std::string& customerId) {
    sqlite3_stmt* stmt;

    // 1. Opțional: șterge toate conturile asociate cu clientul
    std::string deleteAccountsQuery = "DELETE FROM accounts WHERE customer_id = ?;";
    sqlite3_prepare_v2(db, deleteAccountsQuery.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, customerId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // 2. Șterge clientul din tabela customers
    std::string deleteCustomerQuery = "DELETE FROM customers WHERE customer_id = ?;";
    sqlite3_prepare_v2(db, deleteCustomerQuery.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, customerId.c_str(), -1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    return success;
}


bool DatabaseHandler::DeleteAccount(const std::string& accountId) {
    std::string query = "DELETE FROM accounts WHERE account_id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, accountId.c_str(), -1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool DatabaseHandler::DeleteTransactionsForCustomer(const std::string& customerId) {
    // Obține toate account_id-urile asociate clientului
    std::string query = "SELECT account_id FROM accounts WHERE customer_id = ?;";
    sqlite3_stmt* stmt;

    std::vector<std::string> accountIds;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, customerId.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* accId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (accId)
            accountIds.push_back(accId);
    }

    sqlite3_finalize(stmt);

    // Șterge tranzacțiile pentru fiecare cont
    for (const std::string& accId : accountIds) {
        std::string deleteQuery = "DELETE FROM transactions WHERE account_id = ?;";
        sqlite3_stmt* delStmt;

        if (sqlite3_prepare_v2(db, deleteQuery.c_str(), -1, &delStmt, nullptr) != SQLITE_OK)
            continue;

        sqlite3_bind_text(delStmt, 1, accId.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(delStmt);
        sqlite3_finalize(delStmt);
    }

    return true;
}




Customer* DatabaseHandler::LoadCustomer(const std::string& firstName, const std::string& lastName, int password) {
    std::string query = "SELECT * FROM customers WHERE first_name=? AND last_name=? AND password=?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, firstName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, lastName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, password);

    Customer* result = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int age = sqlite3_column_int(stmt, 3);
        int pw = sqlite3_column_int(stmt, 4);
        std::string cnp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        result = new Customer(id, firstName, lastName, age, pw, cnp);
    }

    sqlite3_finalize(stmt);
    return result;
}

bool DatabaseHandler::InsertAccount(const Account& account) {
    std::string query = "INSERT INTO accounts VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, account.GetAccountID().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, account.GetCustomerID().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, account.GetAccountNumber().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, account.GetBalance());

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

std::vector<Account> DatabaseHandler::LoadAccountsForCustomer(const std::string& customerId) {
    std::vector<Account> result;
    std::string query = "SELECT * FROM accounts WHERE customer_id=?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, customerId.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string accId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string custId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string accNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        double balance = sqlite3_column_double(stmt, 3);

        result.emplace_back(accId, custId, accNumber, balance);
    }

    sqlite3_finalize(stmt);
    return result;
}


std::vector<std::string> DatabaseHandler::GetAllCustomerIDs() {
    std::vector<std::string> ids;
    std::string query = "SELECT customer_id FROM customers;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            if (id) ids.emplace_back(id);
        }
    }

    sqlite3_finalize(stmt);
    return ids;
}
Customer* DatabaseHandler::LoadCustomerByID(const std::string& customerId) {
    std::string query = "SELECT * FROM customers WHERE customer_id=?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_text(stmt, 1, customerId.c_str(), -1, SQLITE_TRANSIENT);

    Customer* result = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int age = sqlite3_column_int(stmt, 3);
        int password = sqlite3_column_int(stmt, 4);
        std::string cnp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

        result = new Customer(id, firstName, lastName, age, password, cnp);
    }

    sqlite3_finalize(stmt);
    return result;
}

std::vector<Account> DatabaseHandler::LoadAllAccounts() {
    std::vector<Account> result;
    std::string query = "SELECT * FROM accounts;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string accId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::string custId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string accNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            double balance = sqlite3_column_double(stmt, 3);

            result.emplace_back(accId, custId, accNumber, balance);
        }
    }

    sqlite3_finalize(stmt);
    return result;
}


//Transaction

bool DatabaseHandler::InsertTransaction(const Transaction& transaction) {
    std::string query = "INSERT INTO transactions VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, transaction.GetID().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, transaction.GetAccountID().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, transaction.GetType().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, transaction.GetAmount());
    sqlite3_bind_text(stmt, 5, transaction.GetTimestamp().c_str(), -1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

std::vector<Transaction> DatabaseHandler::GetTransactionsForAccount(const std::string& accountId) {
    std::vector<Transaction> result;
    std::string query = "SELECT * FROM transactions WHERE account_id=?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, accountId.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string accId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        double amount = sqlite3_column_double(stmt, 3);
        std::string timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

        result.emplace_back(id, accId, type, amount, timestamp);
    }

    sqlite3_finalize(stmt);
    return result;
}
std::vector<Transaction> DatabaseHandler::LoadAllTransactions() {
    std::vector<Transaction> result;
    std::string query = "SELECT * FROM transactions;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::string accId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            double amount = sqlite3_column_double(stmt, 3);
            std::string timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

            result.emplace_back(id, accId, type, amount, timestamp);
        }
    }

    sqlite3_finalize(stmt);
    return result;
}
Transaction* DatabaseHandler::LoadTransactionByID(const std::string& transactionId) {
    std::string query = "SELECT * FROM transactions WHERE transaction_id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_text(stmt, 1, transactionId.c_str(), -1, SQLITE_TRANSIENT);

    Transaction* result = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string accId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        double amount = sqlite3_column_double(stmt, 3);
        std::string timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

        result = new Transaction(id, accId, type, amount, timestamp);
    }

    sqlite3_finalize(stmt);
    return result;
}
