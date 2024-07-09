#include "user.h"
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

void User::SetUsername(string s)
{
    username = s;
}
void User::SetPassword(string s)
{
    password = s;
}
void User::SetEmail(string s)
{
    email = s;
}
string User::GetUsername()
{
    return username;
}
string User::GetPassword()
{
    return password;
}
string User::GetEmail()
{
    return email;
}

void User::AddToActivityLog(string s)
{
    qDebug() << "Activity log for " << username << " has been updated";
    this->activityLog.push_back(s);
}

void User::TransferMoney(float amount, BankAccount sender, BankAccount receiver)
{
    sender.send(amount, receiver);
    this->AddToActivityLog("Transferred " + to_string(amount) + " from account " + to_string(sender.getNumber()) + " to account " + to_string(receiver.getNumber()));
}

void User::SendMoney(float amount, BankAccount sendAccount, User receiver)
{
    sendAccount.send(amount, receiver.accounts[0]);
    this->AddToActivityLog("Sent " + to_string(amount) + " from account " + to_string(sendAccount.getNumber()) + " to user " + receiver.GetUsername()); //send money to first account in receiver's list
    receiver.AddToActivityLog("Received " + to_string(amount) + " from user " + this->GetUsername());
}

void User::CreateBankAccount(int accNum, string type, float balance)
{
    this->accounts.push_back(BankAccount(accNum, type, balance)); //create new bank account with $0
    qDebug() << "Created bank account #" + to_string(accNum) + " under user " + this->GetUsername();
    this->AddToActivityLog("Created bank account #" + to_string(accNum));
}

vector<string> User::GetActivityLog()
{
    return activityLog;
}
