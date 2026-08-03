/*
 * ===============================================================
 *  TechNova Internship Program - C++ Programming Track
 *  TASK 2: Bank Account System
 * ===============================================================
 *  Requirements covered:
 *    - Create a bank account class                     [DONE]
 *    - Implement deposit and withdraw functions        [DONE]
 *    - Use encapsulation                                [DONE]
 *    - Display account balance                          [DONE]
 *    - BONUS: Add inheritance                           [DONE]
 *
 *  Design notes:
 *    - BankAccount is the base class. All sensitive data
 *      (balance, account number, holder name, pin) is kept
 *      private and only accessible through public member
 *      functions -> encapsulation.
 *    - SavingsAccount publicly inherits from BankAccount and
 *      adds interest-rate based behaviour (bonus requirement).
 *    - A simple menu-driven console interface lets the user
 *      create accounts, deposit, withdraw, check balance,
 *      apply interest, and view a mini transaction history.
 * ===============================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <limits>

using namespace std;

// ---------------------------------------------------------------
// Base Class: BankAccount
// ---------------------------------------------------------------
class BankAccount {
private:
    // Encapsulated data members - not accessible directly from outside
    string accountNumber;
    string holderName;

protected:
    // 'protected' so derived classes (e.g. SavingsAccount) can use it
    // directly, but it is still hidden from outside code.
    double balance;
    vector<string> transactionHistory;

public:
    // Constructor
    BankAccount(const string& accNo, const string& name, double initialBalance = 0.0)
        : accountNumber(accNo), holderName(name), balance(0.0) {
        if (initialBalance < 0) {
            cout << "[Warning] Initial balance cannot be negative. Set to 0.\n";
        } else {
            balance = initialBalance;
            if (initialBalance > 0)
                logTransaction("Account opened with initial deposit of Rs. " + formatAmount(initialBalance));
        }
        if (transactionHistory.empty())
            logTransaction("Account opened with zero balance.");
    }

    // Virtual destructor - important since we will use base class pointers
    // to hold derived class objects (good practice with inheritance).
    virtual ~BankAccount() {}

    // ---------- Deposit ----------
    virtual bool deposit(double amount) {
        if (amount <= 0) {
            cout << "[Error] Deposit amount must be positive.\n";
            return false;
        }
        balance += amount;
        logTransaction("Deposited Rs. " + formatAmount(amount));
        return true;
    }

    // ---------- Withdraw ----------
    virtual bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "[Error] Withdrawal amount must be positive.\n";
            return false;
        }
        if (amount > balance) {
            cout << "[Error] Insufficient funds. Current balance: Rs. "
                 << fixed << setprecision(2) << balance << "\n";
            return false;
        }
        balance -= amount;
        logTransaction("Withdrew Rs. " + formatAmount(amount));
        return true;
    }

    // ---------- Display Balance ----------
    virtual void displayBalance() const {
        cout << fixed << setprecision(2);
        cout << "\n----------------------------------\n";
        cout << " Account Number : " << accountNumber << "\n";
        cout << " Holder Name    : " << holderName << "\n";
        cout << " Current Balance: Rs. " << balance << "\n";
        cout << "----------------------------------\n";
    }

    // ---------- Transaction History ----------
    void displayHistory() const {
        cout << "\n--- Transaction History for " << accountNumber << " ---\n";
        if (transactionHistory.empty()) {
            cout << "No transactions yet.\n";
            return;
        }
        for (size_t i = 0; i < transactionHistory.size(); ++i) {
            cout << i + 1 << ". " << transactionHistory[i] << "\n";
        }
    }

    // ---------- Getters (read-only access to private data) ----------
    string getAccountNumber() const { return accountNumber; }
    string getHolderName() const { return holderName; }
    double getBalance() const { return balance; }

protected:
    void logTransaction(const string& entry) {
        transactionHistory.push_back(entry);
    }

    string formatAmount(double amount) const {
        char buf[64];
        snprintf(buf, sizeof(buf), "%.2f", amount);
        return string(buf);
    }
};

// ---------------------------------------------------------------
// Derived Class: SavingsAccount  (BONUS: demonstrates inheritance)
// ---------------------------------------------------------------
class SavingsAccount : public BankAccount {
private:
    double interestRate; // annual interest rate, e.g. 0.05 = 5%
    static constexpr double MIN_BALANCE = 500.0; // savings accounts must keep a minimum balance

public:
    SavingsAccount(const string& accNo, const string& name,
                   double initialBalance, double rate)
        : BankAccount(accNo, name, initialBalance), interestRate(rate) {}

    // Override withdraw to enforce the minimum balance rule
    // specific to savings accounts.
    bool withdraw(double amount) override {
        if (amount <= 0) {
            cout << "[Error] Withdrawal amount must be positive.\n";
            return false;
        }
        if (balance - amount < MIN_BALANCE) {
            cout << "[Error] Withdrawal denied. Savings accounts must maintain a minimum balance of Rs. "
                 << fixed << setprecision(2) << MIN_BALANCE << "\n";
            return false;
        }
        balance -= amount;
        logTransaction("Withdrew Rs. " + formatAmount(amount) + " (Savings)");
        return true;
    }

    // New behaviour unique to SavingsAccount
    void applyInterest() {
        double interest = balance * interestRate;
        balance += interest;
        char rateBuf[32];
        snprintf(rateBuf, sizeof(rateBuf), "%.2f", interestRate * 100);
        logTransaction("Interest credited: Rs. " + formatAmount(interest) +
                        " at rate " + string(rateBuf) + "%");
        cout << "[Info] Interest of Rs. " << fixed << setprecision(2)
             << interest << " credited.\n";
    }

    // Override to also show the interest rate (extends base behaviour)
    void displayBalance() const override {
        BankAccount::displayBalance();
        cout << " Account Type   : Savings\n";
        cout << " Interest Rate  : " << (interestRate * 100) << "% per annum\n";
        cout << "----------------------------------\n";
    }
};

// ---------------------------------------------------------------
// Helper functions for the menu-driven console interface
// ---------------------------------------------------------------
void showMenu() {
    cout << "\n============ BANK ACCOUNT SYSTEM ============\n";
    cout << "1. Create Regular Bank Account\n";
    cout << "2. Create Savings Account\n";
    cout << "3. Deposit\n";
    cout << "4. Withdraw\n";
    cout << "5. Display Balance\n";
    cout << "6. Apply Interest (Savings Accounts only)\n";
    cout << "7. View Transaction History\n";
    cout << "8. List All Accounts\n";
    cout << "0. Exit\n";
    cout << "===============================================\n";
    cout << "Enter your choice: ";
}

int findAccountIndex(const vector<shared_ptr<BankAccount>>& accounts, const string& accNo) {
    for (size_t i = 0; i < accounts.size(); ++i) {
        if (accounts[i]->getAccountNumber() == accNo) return static_cast<int>(i);
    }
    return -1;
}

double readAmount() {
    double amt;
    while (!(cin >> amt)) {
        cout << "[Error] Please enter a valid numeric amount: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return amt;
}

int readChoice() {
    int choice;
    while (!(cin >> choice)) {
        cout << "[Error] Please enter a valid number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return choice;
}

// ---------------------------------------------------------------
// Main Program
// ---------------------------------------------------------------
int main() {
    vector<shared_ptr<BankAccount>> accounts;
    int choice;

    cout << "Welcome to the TechNova Bank Account Management System\n";

    do {
        showMenu();
        choice = readChoice();

        switch (choice) {
            case 1: {
                string accNo, name;
                double initial;
                cout << "Enter Account Number: ";
                cin >> accNo;
                cout << "Enter Holder Name (no spaces, or use underscores): ";
                cin >> name;
                cout << "Enter Initial Deposit: ";
                initial = readAmount();
                accounts.push_back(make_shared<BankAccount>(accNo, name, initial));
                cout << "[Success] Regular account created.\n";
                break;
            }
            case 2: {
                string accNo, name;
                double initial, rate;
                cout << "Enter Account Number: ";
                cin >> accNo;
                cout << "Enter Holder Name (no spaces, or use underscores): ";
                cin >> name;
                cout << "Enter Initial Deposit: ";
                initial = readAmount();
                cout << "Enter Annual Interest Rate (e.g. 0.05 for 5%): ";
                rate = readAmount();
                accounts.push_back(make_shared<SavingsAccount>(accNo, name, initial, rate));
                cout << "[Success] Savings account created.\n";
                break;
            }
            case 3: {
                string accNo;
                cout << "Enter Account Number: ";
                cin >> accNo;
                int idx = findAccountIndex(accounts, accNo);
                if (idx == -1) { cout << "[Error] Account not found.\n"; break; }
                cout << "Enter amount to deposit: ";
                double amt = readAmount();
                if (accounts[idx]->deposit(amt))
                    cout << "[Success] Deposit completed.\n";
                break;
            }
            case 4: {
                string accNo;
                cout << "Enter Account Number: ";
                cin >> accNo;
                int idx = findAccountIndex(accounts, accNo);
                if (idx == -1) { cout << "[Error] Account not found.\n"; break; }
                cout << "Enter amount to withdraw: ";
                double amt = readAmount();
                if (accounts[idx]->withdraw(amt))
                    cout << "[Success] Withdrawal completed.\n";
                break;
            }
            case 5: {
                string accNo;
                cout << "Enter Account Number: ";
                cin >> accNo;
                int idx = findAccountIndex(accounts, accNo);
                if (idx == -1) { cout << "[Error] Account not found.\n"; break; }
                accounts[idx]->displayBalance();
                break;
            }
            case 6: {
                string accNo;
                cout << "Enter Account Number: ";
                cin >> accNo;
                int idx = findAccountIndex(accounts, accNo);
                if (idx == -1) { cout << "[Error] Account not found.\n"; break; }
                auto savings = dynamic_pointer_cast<SavingsAccount>(accounts[idx]);
                if (savings) {
                    savings->applyInterest();
                } else {
                    cout << "[Error] Interest can only be applied to savings accounts.\n";
                }
                break;
            }
            case 7: {
                string accNo;
                cout << "Enter Account Number: ";
                cin >> accNo;
                int idx = findAccountIndex(accounts, accNo);
                if (idx == -1) { cout << "[Error] Account not found.\n"; break; }
                accounts[idx]->displayHistory();
                break;
            }
            case 8: {
                if (accounts.empty()) {
                    cout << "[Info] No accounts created yet.\n";
                    break;
                }
                cout << "\n--- All Accounts ---\n";
                for (const auto& acc : accounts) {
                    cout << "Acc#: " << acc->getAccountNumber()
                         << " | Name: " << acc->getHolderName()
                         << " | Balance: Rs. " << fixed << setprecision(2)
                         << acc->getBalance() << "\n";
                }
                break;
            }
            case 0:
                cout << "Thank you for using the TechNova Bank Account System. Goodbye!\n";
                break;
            default:
                cout << "[Error] Invalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}