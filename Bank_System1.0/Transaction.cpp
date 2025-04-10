#include "Transaction.h"

Transaction::Transaction(const std::string& id, const std::string& accountId,
    const std::string& type, double amount, const std::string& timestamp)
    : id(id), accountId(accountId), type(type), amount(amount), timestamp(timestamp) {}

std::string Transaction::GetID() const { return id; }
std::string Transaction::GetAccountID() const { return accountId; }
std::string Transaction::GetType() const { return type; }
double Transaction::GetAmount() const { return amount; }
std::string Transaction::GetTimestamp() const { return timestamp; }

