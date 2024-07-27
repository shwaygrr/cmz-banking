#include "user.h"
#include "widgets/BankWidget.h"
#include "transaction.h"
#include <vector>
#include <QDebug>
#include <string>
#include "api/db.h"

User::User() {
    user_id = 0;
    full_name = "";
    username = "";
    created_at = "";
}

User::User(int user_id_, QString full_name_, QString username_, QString created_at_) {
    user_id = user_id_;
    full_name = full_name_;
    username = username_;
    created_at = created_at_;
}

QDebug operator << (QDebug dbg, const User& user) {
    QDebugStateSaver saver(dbg);
    dbg.nospace();

    if (user.getUserId() == 0) {
        dbg << "Invalid User -- User undefined";
    } else {
        dbg << "User ID: " << user.getUserId() << "\n"
            << "Full Name: " << user.getFullName() << "\n"
            << "Username: " << user.getUsername() << "\n"
            << "Created At: " << user.getCreatedAt();
    }
    return dbg;
}


//add security levels and error handling to getters later
int User::getUserId() const {
    return user_id;
}

QString User::getFullName() const {
    return full_name;
}

QString User::getUsername() const {
    return username;
}

QString User::getCreatedAt() const {
    return created_at;
}

vector<string> User::getActivityLog()
{
    return activityLog;
}

QList<BankAccount> User::getBankAccounts()
{
    return bank_accounts;
}

void User::setActivityLog(vector<string> s)
{
    activityLog = s;
}

void User::setBankAccounts(QList<BankAccount> bank_accounts_) { bank_accounts = bank_accounts_; }

void User::addToActivityLog(string s) {
    qDebug() << s;
    this->activityLog.push_back(s);
}

void User::transferMoney(float amount, BankAccount &sender, BankAccount &receiver)
{
    sender.send(amount, receiver);
    QString strNumber = QString::number(amount, 'f', 2);

    // this->addToActivityLog(Transaction(sender.getAccountNumber(), receiver.getAccountNumber(), amount, time(0)).transferLogEntry());
}

void User::sendMoney(float amount, BankAccount sendAccount, User receiver)
{
    // sendAccount.send(amount, receiver.accounts[0]);   //send money to first account in receiver's list
    // this->addToActivityLog(Transaction(this->getUsername().toStdString(), sendAccount.getNumber(), receiver.getUsername().toStdString(), receiver.getBankAccounts()[0].getNumber(), amount, time(0)).senderLogEntry());
    // receiver.addToActivityLog(Transaction(this->getUsername().toStdString(), sendAccount.getNumber(), receiver.getUsername().toStdString(), receiver.getBankAccounts()[0].getNumber(), amount, time(0)).receiverLogEntry());
}

bool User::createBankAccount(const QString& account_type, const float balance) {
    DB db;
    BankAccount account(user_id, account_type, balance);
    return db.createBankAccount(account);
}

bool User::deleteBankAccount(const QString& account_number) {
    DB db;
    db.deleteBankAccountByNumber(account_number);
}

BankAccount* User::findBankAccount(const QString& account_number) {
    for (BankAccount& bank_account : bank_accounts) {
        if (bank_account.getAccountNumber() == account_number) {
            return &bank_account;
        }
    }

    qDebug() << "Couldn't find account";
    return nullptr;
}

