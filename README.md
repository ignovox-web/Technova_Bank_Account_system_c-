# TechNova_BankAccountSystem

A C++ console application built for the **TechNova Internship Program** — C++ Programming Track (Task 2: Bank Account System).

## 📋 Overview

This project simulates a simple banking system that allows users to create bank accounts, deposit and withdraw money, view balances, and track transaction history — all through an interactive, menu-driven console interface.

It demonstrates core Object-Oriented Programming (OOP) concepts in C++, including **encapsulation** and **inheritance**.

## ✨ Features

- Create **Regular Bank Accounts** and **Savings Accounts**
- Deposit and withdraw funds with input validation (rejects negative/invalid amounts)
- Encapsulated account data (account number, holder name, balance) — accessible only through public methods
- **Savings Account** (bonus feature) inherits from the base `BankAccount` class and adds:
  - Minimum balance enforcement (₹500) on withdrawals
  - Interest calculation and crediting based on a configurable annual rate
- Transaction history log per account
- List all accounts at once
- Robust error handling for invalid menu choices, non-numeric input, insufficient funds, and negative amounts

## 🧠 OOP Concepts Demonstrated

| Concept | Where it's used |
|---|---|
| **Class & Objects** | `BankAccount` and `SavingsAccount` classes |
| **Encapsulation** | Private/protected data members (`balance`, `accountNumber`, `holderName`) accessed only via public methods |
| **Inheritance** | `SavingsAccount` publicly inherits from `BankAccount` |
| **Polymorphism** | `withdraw()` and `displayBalance()` are virtual and overridden in `SavingsAccount` |
| **Constructors** | Used to initialize account data safely on creation |

## 🛠️ Tech Stack

- **Language:** C++ (C++17)
- **Libraries used:** `<iostream>`, `<iomanip>`, `<string>`, `<vector>`, `<memory>`, `<limits>`

## 🚀 How to Compile & Run

```bash
# Clone the repository
git clone https://github.com/<your-username>/TechNova_BankAccountSystem.git
cd TechNova_BankAccountSystem

# Compile
g++ -std=c++17 -Wall -Wextra -o bank_system BankAccountSystem.cpp

# Run
./bank_system
```

On Windows (using g++ via MinGW):
```bash
g++ -std=c++17 -o bank_system.exe BankAccountSystem.cpp
bank_system.exe
```

## 📖 Menu Options

```
1. Create Regular Bank Account
2. Create Savings Account
3. Deposit
4. Withdraw
5. Display Balance
6. Apply Interest (Savings Accounts only)
7. View Transaction History
8. List All Accounts
0. Exit
```

## 🧪 Sample Usage

```
Enter your choice: 1
Enter Account Number: ACC001
Enter Holder Name (no spaces, or use underscores): John_Doe
Enter Initial Deposit: 1000
[Success] Regular account created.
```

## 📂 Project Structure

```
TechNova_BankAccountSystem/
│
├── BankAccountSystem.cpp   # Main source file
└── README.md               # Project documentation
```

## 🎓 About This Project

This project was completed as part of the **TechNova Internship Program (C++ Programming Track)**, fulfilling Task 2: Bank Account System, including the bonus inheritance requirement.

## 📄 License

This project is open for educational and portfolio purposes.
