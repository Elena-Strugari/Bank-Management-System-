#include <iostream>
#include <string>
#include <vector>
#include "Bank.h"

uint8_t TypePerson() {
    std::cout << "Select user type:\n";
    std::cout << "1. CUSTOMER\n";
    std::cout << "2. EMPLOYEE\n";
    std::cout << "Choice: ";
    uint8_t type;
    std::cin >> type;
    return type;
}

// Utility to split full name into first and last
bool SplitFullName(const std::string& fullName, std::string& firstName, std::string& lastName) {
    size_t space = fullName.find(' ');
    if (space == std::string::npos) return false;
    firstName = fullName.substr(0, space);
    lastName = fullName.substr(space + 1);
    return true;
}

int main() {
    Bank bank;

    // Sample data
    // Customer 1: Alice Smith (2 accounts)
    bank.CreateCustomer("C001", "Alice", "Smith", 30, 1234, "1234567890123");
    bank.CreateAccount("A001", "C001", "RO12BANK1234", 1000.0);
    bank.CreateAccount("A002", "C001", "RO12BANK5678", 250.0);

    // Customer 2: John Miller (1 account)
    bank.CreateCustomer("C002", "John", "Miller", 45, 5678, "2234567890123");
    bank.CreateAccount("A003", "C002", "RO22BANK3456", 5400.0);

    // Customer 3: Maria Popescu (3 accounts)
    bank.CreateCustomer("C003", "Maria", "Popescu", 27, 1111, "3234567890123");
    bank.CreateAccount("A004", "C003", "RO33BANK9999", 1200.0);
    bank.CreateAccount("A005", "C003", "RO33BANK8888", 75.0);
    bank.CreateAccount("A006", "C003", "RO33BANK7777", 4000.0);

    // Customer 4: Daniel Ionescu (1 account)
    bank.CreateCustomer("C004", "Daniel", "Ionescu", 39, 2222, "4234567890123");
    bank.CreateAccount("A007", "C004", "RO44BANK6666", 50.0);

    // Customer 5: Ana Stoica (2 accounts)
    bank.CreateCustomer("C005", "Ana", "Stoica", 22, 3333, "5234567890123");
    bank.CreateAccount("A008", "C005", "RO55BANK5555", 700.0);
    bank.CreateAccount("A009", "C005", "RO55BANK4444", 1500.0);

   // bank.DeleteAll();



    std::cout << "\n===========================\n";
    std::cout << "    BANKING RECORD SYSTEM\n";
    std::cout << "===========================\n\n";

    uint8_t typePerson = TypePerson();

    if (typePerson == '1') { // CUSTOMER
        std::string fullName, firstName, lastName;
        uint16_t password;

        std::cout << "\nEnter your details:\n";
        std::cin.ignore(); // Clear leftover newline
        std::cout << "Full name (First Last, e.g., Ana Popescu): ";
        std::getline(std::cin, fullName);

        if (!SplitFullName(fullName, firstName, lastName)) {
            std::cout << "Invalid format! Please use: FirstName LastName\n";
            return 1;
        }

        std::cout << "Password: ";
        std::cin >> password;

        Customer* customer = bank.LoginCustomer(firstName, lastName, password);
        if (!customer) {
            std::cout << "Login failed. Incorrect name or password.\n";
            return 1;
        }

        std::cout << "\nWelcome, " << firstName << "!\n";

        int choice;
        do {
            std::cout << "\n==== CUSTOMER MENU ====\n";
            std::cout << "1. View My Accounts\n";
            std::cout << "2. Deposit\n";
            std::cout << "3. Withdraw\n";
            std::cout << "4. See your transactions\n";
            std::cout << "5. About me\n";
            std::cout << "6. Export Statement txt\n";
            std::cout << "7. Exit\n";
            std::cout << "Choose: ";
            std::cin >> choice;

            if (choice == 1) {
                auto accounts = bank.GetAccounts(customer->GetCustomerID());
                if (accounts.empty()) {
                    std::cout << "You have no accounts.\n";
                }
                else {
                    std::cout << "Your accounts:\n";
                    for (const auto* acc : accounts) {
                        std::cout <<acc->GetAccountID()<< "- " << acc->GetAccountNumber()
                            << " | Balance: " << acc->GetBalance() << "\n";
                    }
                }
            }
            else if (choice == 2) {
                std::string accId;
                double amount;
                std::cout << "Account ID: ";
                std::cin >> accId;
                std::cout << "Deposit amount: ";
                std::cin >> amount;

                if (bank.Deposit(accId, amount)) {
                    std::cout << "Deposit successful.\n";
                }
                else {
                    std::cout << "Invalid account.\n";
                }
            }
            else if (choice == 3) {
                std::string accId;
                double amount;
                std::cout << "Account ID: ";
                std::cin >> accId;
                std::cout << "Withdraw amount: ";
                std::cin >> amount;

                if (bank.Withdraw(accId, amount)) {
                    std::cout << "Withdrawal successful.\n";
                }
                else {
                    std::cout << "Insufficient funds or account not found.\n";
                }
            }
            else if (choice == 4) {
                bank.TransactionId(customer->GetCustomerID());
            }
            else if (choice == 5) {
               
                bank.GetCustomerData(customer->GetCustomerID(), true, false);
            }
            else if (choice == 6) {

                bank.GetCustomerData(customer->GetCustomerID(), false, true);
            }

        } while (choice != 7);
    }
    else if (typePerson == '2') { // EMPLOYEE
        int empChoice;
        do {
            std::cout << "\n==== EMPLOYEE MENU ====\n";
            std::cout << "1. Create new customer\n";
            std::cout << "2. Create new account\n";
            std::cout << "3. Show all customers\n";
            std::cout << "4. Show all accounts\n";
            std::cout << "5. Show all transactions\n";
            std::cout << "6. Delete Customer\n";
            std::cout << "7. Delete Account\n";
            std::cout << "8. Exit\n";
            std::cout << "Choose: ";
            std::cin >> empChoice;

            if (empChoice == 1) {
                std::string id, fn, ln, cnp;
                int age, pw;
                std::cout << "Customer ID: ";
                std::cin >> id;
                std::cout << "First Name: ";
                std::cin >> fn;
                std::cout << "Last Name: ";
                std::cin >> ln;
                std::cout << "Age: ";
                std::cin >> age;
                std::cout << "Password: ";
                std::cin >> pw;
                std::cout << "CNP: ";
                std::cin >> cnp;

                if (bank.CreateCustomer(id, fn, ln, age, pw, cnp)) {
                    std::cout << "Customer created successfully.\n";
                }
                else {
                    std::cout << "Customer already exists.\n";
                }
            }
            else if (empChoice == 2) {
                std::string accId, custId, accNum;
                double bal;
                std::cout << "Account ID: ";
                std::cin >> accId;
                std::cout << "Customer ID: ";
                std::cin >> custId;
                std::cout << "Account number: ";
                std::cin >> accNum;
                std::cout << "Initial balance: ";
                std::cin >> bal;

                if (bank.CreateAccount(accId, custId, accNum, bal)) {
                    std::cout << "Account created successfully.\n";
                }
                else {
                    std::cout << "Error creating account.\n";
                }
            }
            else if (empChoice == 3) {
                bank.ShowAllCustomers();
            }
            else if (empChoice == 4) {
                bank.ShowAllAccounts();
            }
            else if (empChoice == 5) {
                std::vector<Transaction> tx= bank.GetAllTransactions();
                std::cout << "\n=== TRANSACTION HISTORY ===\n";
                for (const auto& t : tx) {
                    std::cout << t.GetTimestamp() << " | " << t.GetID()
                        << " | Account: " << t.GetAccountID()
                        << " | " << t.GetType() << " | Amount: " << t.GetAmount() << "\n";
                }
            }
            else if (empChoice == 6) {
                std::cout << "Insert Customer Id: ";
                std::string id;
                std::cin >> id;
                bank.DeleteAllCustomer(id);

            }
            else if (empChoice == 7) {
                std::cout << "Insert Customer Id: ";
                std::string id;
                std::cin >> id;
                bank.DeleteAccount(id);
            }


        } while (empChoice != 8);
    }

    std::cout << "\nProgram ended.\n";
    return 0;
}
