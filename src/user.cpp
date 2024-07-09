#include "user.h"
#include <vector>
#include <QDebug>


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
    qDebug() << "Activity log for " << username << " has been updated: " << s;
    this->activityLog.push_back(s);
}

void User::CreateBankAccount(int accNum, string type, float balance)
{
    accounts.push_back(BankAccount(accNum, type, balance));
}

BankAccount User::FindBankAccount(int num)
{
    for(int i = 0; i < (int)accounts.size(); i++)
    {
        qDebug() << accounts.at(i).getNumber() << " ? " << num;
        if(accounts.at(i).getNumber() == num)
        {
            return accounts.at(i);
        }
    }
    qDebug() << "Couldn't find account";
}

vector<string> User::GetActivityLog()
{
    return activityLog;
}

vector<BankAccount> User::GetAccounts()
{
    return accounts;
}
