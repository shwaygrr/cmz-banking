#include "user.h"
#include "BankWidget.h"
#include <vector>
#include <QDebug>
#include <string>

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
    created_at = created_at;
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

vector<BankAccount> User::getBankAccounts()
{
    return accounts;
}

void User::setActivityLog(vector<string> s)
{
    activityLog = s;
}

void User::setBankAccounts(vector<BankAccount> a)
{
    accounts = a;
}

void User::addToActivityLog(string s)
{
    qDebug() << s;
    this->activityLog.push_back(s);
}

void User::transferMoney(float amount, BankAccount &sender, BankAccount &receiver)
{
    sender.send(amount, receiver);
    QString strNumber = QString::number(amount, 'f', 2);
    this->addToActivityLog("Transferred $" + strNumber.toStdString() + " from account " + to_string(sender.getNumber()) + " to account " + to_string(receiver.getNumber()));
}

void User::sendMoney(float amount, BankAccount sendAccount, User receiver)
{
    sendAccount.send(amount, receiver.accounts[0]);
    this->addToActivityLog("Sent " + to_string(amount) + " from account " + to_string(sendAccount.getNumber()) + " to user " + receiver.getUsername()); //send money to first account in receiver's list
    receiver.addToActivityLog("Received " + to_string(amount) + " from user " + this->getUsername());
}

void User::createBankAccount(int accNum, string type, float balance)
{
    this->accounts.push_back(BankAccount(accNum, type, balance)); //create new bank account with $0
}

void User::deleteBankAccount(int accNum)
{
    for (int i=0; i < (int)this->accounts.size(); i++) {
        if (accNum == this->accounts[i].getNumber()) {
            this->accounts.erase(accounts.begin() + i);
            this->addToActivityLog("Closed account #" + to_string(accNum));
            qDebug() << "User " << this->getUsername() << " closed account #" << to_string(accNum);
        }
    }
}

BankAccount* User::findBankAccount(int num)
{
    for(int i = 0; i < (int)accounts.size(); i++)
    {
        if(accounts.at(i).getNumber() == num)
        {
            return &accounts.at(i);
        }
    }
    qDebug() << "Couldn't find account";
}
