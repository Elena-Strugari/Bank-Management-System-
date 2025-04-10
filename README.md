# Bank Management System – C++ & SQLite

A modular C++ desktop application that simulates basic banking operations, using an SQLite database for persistent data storage.

## Features

- Create and manage customer accounts
- Process deposits, withdrawals, and transfers
- Track all transactions per account
- Store data in local SQLite databases (`bank.db`, `bank1.db`, etc.)
- Fully object-oriented design: separate classes for `Account`, `Customer`, `Transaction`, and `Bank`

## Technologies Used

- C++
- SQLite (embedded database)
- Visual Studio
- File I/O for initial data
- OOP Design Principles

## Project Structure
Bank_System1.0/
├── Bank_System1.0.sln                # Visual Studio solution file
├── Bank_System1.0/                   # Main application source code
│   ├── Account.cpp / Account.h       # Class for managing accounts
│   ├── Bank.cpp / Bank.h             # Core logic class for the bank system
│   ├── Customer.cpp / Customer.h     # Class for managing customer information
│   ├── Transaction.cpp / Transaction.h # Class for handling transactions
│   ├── DatabaseHandler.cpp / .h      # Interface for SQLite database operations
│   ├── BankSystem.cpp                # Main application entry point
│   ├── bank.db / bank1.db / bank2.db # Sample SQLite database files
│   ├── Bank_System1.0.vcxproj        # Visual Studio project configuration
│   └── x64/Debug/...                 # Build artifacts and compiler output

## How to Run

1. Open `Bank_System1.0.sln` in Visual Studio.
2. Build the solution.
3. Run the executable – the program will interact with the SQLite database and allow command-line operations.

## Learning Outcomes

- OOP architecture for real-world domains
- Integrating SQLite in C++ projects
- Working with persistent storage and database queries
- Structuring C++ code for maintainability

