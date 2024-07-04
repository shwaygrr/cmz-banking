#include "user.h"



User::User(string u, string p)
{
    username = u;
    password = p;
}

void User::SetUsername(string s)
{
    username = s;
}
void User::SetPassword(string s)
{
    password = s;
}
string User::GetUsername()
{
    return username;
}
string User::GetPassword()
{
    return password;
}

