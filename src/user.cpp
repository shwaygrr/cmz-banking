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

vector<string> User::GetActivityLog()
{
    return activityLog;
}

vector<BankAccount> User::GetBankAccounts()
{
    return accounts;
}

void User::SetActivityLog(vector<string> s)
{
    activityLog = s;
}

void User::SetBankAccounts(vector<BankAccount> a)
{
    accounts = a;
}

void User::AddToActivityLog(string s)
{
    qDebug() << s;
    this->activityLog.push_back(s);
}


void User::TransferMoney(float amount, BankAccount &sender, BankAccount &receiver)
{
    sender.send(amount, receiver);
    QString strNumber = QString::number(amount, 'f', 2);
    this->AddToActivityLog("Transferred $" + strNumber.toStdString() + " from account " + to_string(sender.getNumber()) + " to account " + to_string(receiver.getNumber()));
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
}

void User::DeleteBankAccount(int accNum)
{
    for (int i=0; i < (int)this->accounts.size(); i++) {
        if (accNum == this->accounts[i].getNumber()) {
            this->accounts.erase(accounts.begin() + i);
            this->AddToActivityLog("Closed account #" + to_string(accNum));
            qDebug() << "User " << this->GetUsername() << " closed account #" << to_string(accNum);
        }
    }
}

vector<BankWidget*> User::GetBankWidgets()
{
    return widgets;
}

void User::AddBankWidget(BankWidget *widget)
{
    widgets.push_back(widget);
}

BankAccount* User::FindBankAccount(int num)
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

