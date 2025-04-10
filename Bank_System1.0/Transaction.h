#pragma once
#include <string>

class Transaction {
public:
    Transaction(const std::string& id, const std::string& accountId,
        const std::string& type, double amount, const std::string& timestamp);

    std::string GetID() const;
    std::string GetAccountID() const;
    std::string GetType() const;
    double GetAmount() const;
    std::string GetTimestamp() const;

private:
    std::string id, accountId, type, timestamp;
    double amount;
};
