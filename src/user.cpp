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
    qDebug() << "Activity log for " << username << " has been updated";
    this->activityLog.push_back(s);
}

vector<string> User::GetActivityLog()
{
    return activityLog;
}
