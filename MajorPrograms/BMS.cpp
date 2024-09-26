#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <sstream> // Include for stringstream

using namespace std;

// Enum for account types
enum AccountType {
    SAVINGS,
    CHECKING,
    BUSINESS
};

// Class to represent a Transaction
class Transaction {
public:
    string type; // "Deposit", "Withdraw", "Transfer", or "Interest"
    double amount;
    string timestamp;

    Transaction(string t, double amt) : type(t), amount(amt) {
        // Get current time
        time_t now = time(0);
        char* dt = ctime(&now);
        timestamp = string(dt).substr(0, 24); // Trim newline character
    }

    void display() const {
        cout << setw(15) << left << type
             << setw(10) << left << "$" + to_string(amount)
             << timestamp << endl;
    }
};

// Class to represent a Bank Account
class BankAccount {
public:
    string accountHolder;
    string accountNumber;
    double balance;
    AccountType accountType;
    vector<Transaction> transactions;

    BankAccount(string holder, string number, AccountType type)
        : accountHolder(holder), accountNumber(number), balance(0.0), accountType(type) {}

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            transactions.emplace_back("Deposit", amount);
            cout << "Deposited: $" << amount << endl;
        } else {
            cout << "Invalid deposit amount." << endl;
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            transactions.emplace_back("Withdraw", amount);
            cout << "Withdrew: $" << amount << endl;
        } else {
            cout << "Invalid withdrawal amount." << endl;
        }
    }

    void transfer(BankAccount& toAccount, double amount) {
        if (amount > 0 && amount <= balance) {
            withdraw(amount);
            toAccount.deposit(amount);
            transactions.emplace_back("Transfer", amount);
            cout << "Transferred: $" << amount << " to " << toAccount.accountNumber << endl;
        } else {
            cout << "Invalid transfer amount." << endl;
        }
    }

    void display() const {
        cout << setw(20) << left << "Account Holder: " << accountHolder << endl;
        cout << setw(20) << left << "Account Number: " << accountNumber << endl;
        cout << setw(20) << left << "Account Type: " 
             << (accountType == SAVINGS ? "Savings" : accountType == CHECKING ? "Checking" : "Business") << endl;
        cout << setw(20) << left << "Balance: $" << fixed << setprecision(2) << balance << endl;
        cout << setw(20) << left << "Transactions:" << endl;
        cout << setw(15) << left << "Type"
             << setw(10) << left << "Amount"
             << "Date & Time" << endl;
        for (const auto& transaction : transactions) {
            transaction.display();
        }
    }

    void editAccountHolder(const string& newHolder) {
        accountHolder = newHolder;
        cout << "Account holder name updated to: " << accountHolder << endl;
    }

    void updateAccountType(AccountType newType) {
        accountType = newType;
        cout << "Account type updated to: " 
             << (newType == SAVINGS ? "Savings" : newType == CHECKING ? "Checking" : "Business") << endl;
    }
};

// Class to manage Bank Accounts
class Bank {
private:
    vector<BankAccount> accounts;

public:
    void addAccount(const BankAccount& account) {
        accounts.push_back(account);
    }

    void deleteAccount(const string& accountNumber) {
        auto it = remove_if(accounts.begin(), accounts.end(),
            [&](const BankAccount& account) { return account.accountNumber == accountNumber; });
        if (it != accounts.end()) {
            accounts.erase(it, accounts.end());
            cout << "Account " << accountNumber << " deleted successfully." << endl;
        } else {
            cout << "Account not found." << endl;
        }
    }

    void displayAccounts() const {
        for (const auto& account : accounts) {
            account.display();
            cout << string(40, '-') << endl;
        }
    }

    void displayAccountsByHolder(const string& holder) const {
        cout << "Accounts for " << holder << ":" << endl;
        for (const auto& account : accounts) {
            if (account.accountHolder == holder) {
                account.display();
                cout << string(40, '-') << endl;
            }
        }
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& account : accounts) {
                file << account.accountHolder << "," << account.accountNumber << "," << account.balance << "," << account.accountType << endl;
                for (const auto& transaction : account.transactions) {
                    file << transaction.type << "," << transaction.amount << "," << transaction.timestamp << endl;
                }
                file << "ENDTRANSACTION" << endl; // Marker for end of transactions
            }
            file.close();
            cout << "Accounts saved to " << filename << endl;
        } else {
            cout << "Unable to open file for writing." << endl;
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            accounts.clear();
            string holder, number, line;
            double balance;
            int typeInt;
            while (getline(file, holder, ',')) {
                getline(file, number, ',');
                file >> balance;
                file.ignore(); // Ignore the newline character
                file >> typeInt;
                file.ignore(); // Ignore the newline character
                AccountType type = static_cast<AccountType>(typeInt);
                BankAccount account(holder, number, type);
                account.balance = balance;
                while (getline(file, line) && line != "ENDTRANSACTION") {
                    stringstream ss(line);
                    string type, timestamp;
                    double amount;
                    getline(ss, type, ',');
                    ss >> amount;
                    ss.ignore();
                    getline(ss, timestamp);
                    account.transactions.emplace_back(type, amount);
                }
                accounts.push_back(account);
            }
            file.close();
            cout << "Accounts loaded from " << filename << endl;
        } else {
            cout << "Unable to open file for reading." << endl;
        }
    }

    BankAccount* findAccount(const string& accountNumber) {
        for (auto& account : accounts) {
            if (account.accountNumber == accountNumber) {
                return &account;
            }
        }
        return nullptr;
    }

    void displayAccountHistory(const string& accountNumber) {
        auto account = findAccount(accountNumber);
        if (account) {
            account->display();
        } else {
            cout << "Account not found." << endl;
        }
    }

    void calculateInterest(double interestRate) {
        for (auto& account : accounts) {
            double interest = account.balance * (interestRate / 100);
            account.balance += interest;
            account.transactions.emplace_back("Interest", interest);
            cout << "Interest of $" << interest << " applied to account " << account.accountNumber << endl;
        }
    }

    void editAccount(const string& accountNumber) {
        BankAccount* account = findAccount(accountNumber);
        if (account) {
            string newHolder;
            int newType;

            cout << "Enter new account holder name (or press Enter to keep current): ";
            cin.ignore(); // Clear the input buffer
            getline(cin, newHolder);
            if (!newHolder.empty()) {
                account->editAccountHolder(newHolder);
            }

            cout << "Select new account type (0: Savings, 1: Checking, 2: Business, or -1 to keep current): ";
            cin >> newType;
            if (newType >= 0 && newType <= 2) {
                account->updateAccountType(static_cast<AccountType>(newType));
            }

        } else {
            cout << "Account not found." << endl;
        }
    }

    void viewTotalAssets() const {
        double total = 0.0;
        for (const auto& account : accounts) {
            total += account.balance;
        }
        cout << "Total assets in the bank: $" << fixed << setprecision(2) << total << endl;
    }
};

// Function to display the menu and get user choice
int displayMenu() {
    int choice;
    cout << "\nSimple Banking System" << endl;
    cout << "1. Create Account" << endl;
    cout << "2. View All Accounts" << endl;
    cout << "3. Deposit Money" << endl;
    cout << "4. Withdraw Money" << endl;
    cout << "5. Transfer Money" << endl;
    cout << "6. Save Accounts" << endl;
    cout << "7. Load Accounts" << endl;
    cout << "8. Delete Account" << endl;
    cout << "9. View Account History" << endl;
    cout << "10. Calculate Interest" << endl;
    cout << "11. View Accounts by Holder" << endl;
    cout << "12. Edit Account Details" << endl;
    cout << "13. View Total Assets" << endl;
    cout << "14. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    return choice;
}

// Function to authenticate user (dummy implementation)
bool authenticateUser() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password; // In a real system, use hashed passwords
    return username == "admin" && password == "password"; // Dummy check
}

// Main function
int main() {
    Bank bank;
    int choice;

    // Authenticate user
    if (!authenticateUser()) {
        cout << "Authentication failed. Exiting..." << endl;
        return 1;
    }

    do {
        choice = displayMenu();

        switch (choice) {
        case 1: {
            string holder, number;
            int accountType;
            cout << "Enter account holder name: ";
            cin >> ws; // To consume any leading whitespace
            getline(cin, holder);
            cout << "Enter account number: ";
            getline(cin, number);
            cout << "Select account type (0: Savings, 1: Checking, 2: Business): ";
            cin >> accountType;

            // Validate account type
            if (accountType < 0 || accountType > 2) {
                cout << "Invalid account type." << endl;
                break;
            }

            bank.addAccount(BankAccount(holder, number, static_cast<AccountType>(accountType)));
            cout << "Account created successfully." << endl;
            break;
        }
        case 2:
            bank.displayAccounts();
            break;
        case 3: {
            string number;
            double amount;
            cout << "Enter account number for deposit: ";
            cin >> ws;
            getline(cin, number);
            cout << "Enter amount to deposit: $";
            cin >> amount;

            BankAccount* account = bank.findAccount(number);
            if (account) {
                account->deposit(amount);
            } else {
                cout << "Account not found." << endl;
            }
            break;
        }
        case 4: {
            string number;
            double amount;
            cout << "Enter account number for withdrawal: ";
            cin >> ws;
            getline(cin, number);
            cout << "Enter amount to withdraw: $";
            cin >> amount;

            BankAccount* account = bank.findAccount(number);
            if (account) {
                account->withdraw(amount);
            } else {
                cout << "Account not found." << endl;
            }
            break;
        }
        case 5: {
            string fromNumber, toNumber;
            double amount;
            cout << "Enter account number to transfer from: ";
            cin >> ws;
            getline(cin, fromNumber);
            cout << "Enter account number to transfer to: ";
            cin >> ws;
            getline(cin, toNumber);
            cout << "Enter amount to transfer: $";
            cin >> amount;

            BankAccount* fromAccount = bank.findAccount(fromNumber);
            BankAccount* toAccount = bank.findAccount(toNumber);
            if (fromAccount && toAccount) {
                fromAccount->transfer(*toAccount, amount);
            } else {
                cout << "One or both accounts not found." << endl;
            }
            break;
        }
        case 6: {
            string filename;
            cout << "Enter filename to save accounts: ";
            cin >> filename;
            bank.saveToFile(filename);
            break;
        }
        case 7: {
            string filename;
            cout << "Enter filename to load accounts: ";
            cin >> filename;
            bank.loadFromFile(filename);
            break;
        }
        case 8: {
            string number;
            cout << "Enter account number to delete: ";
            cin >> ws;
            getline(cin, number);
            bank.deleteAccount(number);
            break;
        }
        case 9: {
            string number;
            cout << "Enter account number to view history: ";
            cin >> ws;
            getline(cin, number);
            bank.displayAccountHistory(number);
            break;
        }
        case 10: {
            double interestRate;
            cout << "Enter interest rate (in %): ";
            cin >> interestRate;
            bank.calculateInterest(interestRate);
            break;
        }
        case 11: {
            string holder;
            cout << "Enter account holder name to view accounts: ";
            cin >> ws;
            getline(cin, holder);
            bank.displayAccountsByHolder(holder);
            break;
        }
        case 12: {
            string number;
            cout << "Enter account number to edit: ";
            cin >> ws;
            getline(cin, number);
            bank.editAccount(number);
            break;
        }
        case 13:
            bank.viewTotalAssets();
            break;
        case 14:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    } while (choice != 14);

    return 0;
}
