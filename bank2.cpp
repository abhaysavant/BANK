#include <iostream>
#include <string>

using namespace std;

class BankAccount {
protected:
    string accountNumber;
    string accountHolderName;
    double balance;

public:
    BankAccount(string accNum, string accHolder) 
        : accountNumber(accNum), accountHolderName(accHolder), balance(0.0) {}

    virtual void deposit(double amount) {
        balance += amount;
        cout << "Deposited: " << amount << " to account: " << accountNumber << endl;
    }

    virtual void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            cout << "Withdrew: " << amount << " from account: " << accountNumber << endl;
        } else {
            cout << "Insufficient funds for withdrawal from account: " << accountNumber << endl;
        }
    }

    double getBalance() const {
        return balance;
    }

    string getAccountNumber() const {
        return accountNumber;
    }

    virtual void displayAccountInfo() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolderName << endl;
        cout << "Balance: " << balance << endl;
    }

    virtual ~BankAccount() {}
};

class SavingsAccount : public BankAccount {
private:
    double interestRate;

public:
    SavingsAccount(string accNum, string accHolder, double rate) 
        : BankAccount(accNum, accHolder), interestRate(rate) {}

    void calculateInterest() {
        double interest = balance * (interestRate / 100);
        deposit(interest);
        cout << "Interest of " << interest << " added to account: " << accountNumber << endl;
    }

    void displayAccountInfo() const override {
        BankAccount::displayAccountInfo();
        cout << "Interest Rate: " << interestRate << "%" << endl;
    }
};

class CheckingAccount : public BankAccount {
private:
    double overdraftLimit;

public:
    CheckingAccount(string accNum, string accHolder, double limit) 
        : BankAccount(accNum, accHolder), overdraftLimit(limit) {}

    void withdraw(double amount) override {
        if (amount <= balance + overdraftLimit) {
            balance -= amount;
            cout << "Withdrew: " << amount << " from account: " << accountNumber << endl;
        } else {
            cout << "Overdraft limit exceeded for account: " << accountNumber << endl;
        }
    }

    void displayAccountInfo() const override {
        BankAccount::displayAccountInfo();
        cout << "Overdraft Limit: " << overdraftLimit << endl;
    }
};

class FixedDepositAccount : public BankAccount {
private:
    int term; 
    double interestRate;

public:
    FixedDepositAccount(string accNum, string accHolder, int termMonths, double rate) 
        : BankAccount(accNum, accHolder), term(termMonths), interestRate(rate) {}

    void calculateInterest() {
        double interest = balance * (interestRate / 100) * (term / 12.0);
        deposit(interest);
        cout << "Interest of " << interest << " added to account: " << accountNumber << endl;
    }

    void displayAccountInfo() const override {
        BankAccount::displayAccountInfo();
        cout << "Term: " << term << " months, Interest Rate: " << interestRate << "%" << endl;
    }
};

// Menu-driven interface
void displayMenu() {
    cout << "Banking System Menu " << endl;
    cout << "1. Create Savings Account" << endl;
    cout << "2. Create Checking Account" << endl;
    cout << "3. Create Fixed Deposit Account" << endl;
    cout << "4. Deposit" << endl;
    cout << "5. Withdraw" << endl;
    cout << "6. Calculate Interest" << endl;
    cout << "7. Display Account Info" << endl;
    cout << "8. Exit" << endl;
}

int main() {
    const int MAX_ACCOUNTS = 10; 
    BankAccount* accounts[MAX_ACCOUNTS]; 
    int accountCount = 0; 
    int choice;
    string accNum, accHolder;
    double amount, rate;
    int term;

    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: if (accountCount < MAX_ACCOUNTS) {
                    cout << "Enter Account Number: ";
                    cin >> accNum;
                    cout << "Enter Account Holder Name: ";
                    cin >> accHolder;
                    cout << "Enter Interest Rate: ";
                    cin >> rate;
                    accounts[accountCount++] = new SavingsAccount(accNum, accHolder, rate);
                } else {
                    cout << "Maximum account limit reached." << endl;
                }
                break;
            case 2: 
                if (accountCount < MAX_ACCOUNTS) {
                    cout << "Enter Account Number: ";
                    cin >> accNum;
                    cout << "Enter Account Holder Name: ";
                    cin >> accHolder;
                    cout << "Enter Overdraft Limit: ";
                    cin >> rate;
                    accounts[accountCount++] = new CheckingAccount(accNum, accHolder, rate);
                } else {
                    cout << "Maximum account limit reached." << endl;
                }
                break;
            case 3: 
                if (accountCount < MAX_ACCOUNTS) {
                    cout << "Enter Account Number: ";
                    cin >> accNum;
                    cout << "Enter Account Holder Name: ";
                    cin >> accHolder;
                    cout << "Enter Term (in months): ";
                    cin >> term;
                    cout << "Enter Interest Rate: ";
                    cin >> rate;
                    accounts[accountCount++] = new FixedDepositAccount(accNum, accHolder, term, rate);
                } else {
                    cout << "Maximum account limit reached." << endl;
                }
                break;
            case 4: 
                cout << "Enter Account Number: ";
                cin >> accNum;
                cout << "Enter Amount to Deposit: ";
                cin >> amount;
                for (int i = 0; i < accountCount; ++i) {
                    if (accounts[i]->getAccountNumber() == accNum) {
                        accounts[i]->deposit(amount);
                        break;
                    }
                }
                break;
            case 5: 
                cout << "Enter Account Number: ";
                cin >> accNum;
                cout << "Enter Amount to Withdraw: ";
                cin >> amount;
                for (int i = 0; i < accountCount; ++i) {
                    if (accounts[i]->getAccountNumber() == accNum) {
                        accounts[i]->withdraw(amount);
                        break;
                    }
                }
                break;
            case 6: 
                cout << "Enter Account Number: ";
                cin >> accNum;
                for (int i = 0; i < accountCount; ++i) {
                    if (accounts[i]->getAccountNumber() == accNum) {
                        if (SavingsAccount* sa = dynamic_cast<SavingsAccount*>(accounts[i])) {
                            sa->calculateInterest();
                        } else if (FixedDepositAccount* fda = dynamic_cast<FixedDepositAccount*>(accounts[i])) {
                            fda->calculateInterest();
                        } else {
                            cout << "Interest calculation not applicable for this account type." << endl;
                        }
                        break;
                    }
                }
                break;
            case 7: // Display Account Info
                cout << "Enter Account Number: ";
                cin >> accNum;
                for (int i = 0; i < accountCount; ++i) {
                    if (accounts[i]->getAccountNumber() == accNum) {
                        accounts[i]->displayAccountInfo();
                        break;
                    }
                }
                break;
            case 8: // Exit
                cout << "Exiting the banking system." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 8);

    for (int i = 0; i < accountCount; ++i) {
        delete accounts[i];
    }

    return 0;
}