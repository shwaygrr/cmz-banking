#include "user.h"
#include "BankWidget.h"
#include <vector>
#include <QDebug>
#include <string>


User::User()
{
    username = "DEFAULT_NAME";
    password = "DEFAULT_PASSWORD";
    email = "DEFAULT_EMAIL@w.com";
    vector<string> activityLog;
    vector<BankAccount> accounts;
}

User::User(string u, string p, string e)
{
    username = u;
    password = p;
    email = e;
    vector<string> activityLog;
    vector<BankAccount> accounts;
}

User::User(string u, string p, string e, vector<string> l, vector<BankAccount> a)
{
    username = u;
    password = p;
    email = e;
    vector<string> activityLog = l;
    vector<BankAccount> accounts = a;
}

void User::setUsername(string s)
{
    username = s;
}
void User::setPassword(string s)
{
    password = s;
}
void User::setEmail(string s)
{
    email = s;
}
string User::getUsername()
{
    return username;
}
string User::getPassword()
{
    return password;
}
string User::getEmail()
{
    return email;
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
