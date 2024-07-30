#include "bankaccount.h"

BankAccount::BankAccount() : account_id(0), user_id(0), balance(0.0) {}

BankAccount::BankAccount(int user_id_, QString account_type_, float balance_)
    : account_type(account_type_), balance(balance_), user_id(user_id_) {
    account_number = generateAccountNumber();
}

BankAccount::BankAccount(int account_id_, int user_id_, QString account_number_, QString account_type_, float balance_, QString created_at_) {
    account_id = account_id_;
    user_id = user_id_;
    account_number = account_number_;
    account_type = account_type_;
    balance = balance_;
    created_at = created_at_;
}

QString BankAccount::generateAccountNumber() {
    //need make checks to make sure account number doesnt exists in db
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000000000, 9999999999);
    return QString::number(dis(gen));
}


void BankAccount::setAccountType(QString account_type_) { account_type = account_type_; }
void BankAccount::setBalance(float balance_) { balance = balance_; }


QString BankAccount::getAccountNumber() const { return account_number; }
int BankAccount::getAccountId() const { return account_id; }
int BankAccount::getAccountUserId() const { return user_id; }
QString BankAccount::getAccountType() const { return account_type; }
float BankAccount::getBalance() const { return balance; }
QString BankAccount::getCreatedAt() const { return created_at; }

void BankAccount::send(float amount, BankAccount &receiver) {
    balance -= amount;
    receiver.receive(amount);
}

void BankAccount::receive(float amount) {
    balance += amount;
}

QDebug operator << (QDebug dbg, const BankAccount& account) {
    QDebugStateSaver saver(dbg);
    dbg.nospace();

    if (account.getAccountId() == 0) {
        dbg << "Invalid BankAccount -- Account undefined";
    } else {
        dbg << "Account ID: " << account.getAccountId() << "\n"
            << "User ID: " << account.getAccountUserId() << "\n"
            << "Account Number: " << account.getAccountNumber() << "\n"
            << "Account Type: " << account.getAccountType() << "\n"
            << "Balance: " << account.getBalance() << "\n"
            << "Created At: " << account.getCreatedAt();
    }
    return dbg;
}
