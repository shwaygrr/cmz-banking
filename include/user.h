#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>

using namespace std;

//this is where you put helper function declarations

class User{
    private:
        string username, password, email;
    public:
        User(string u, string p, string e);

        void SetUsername(string s);
        void SetPassword(string s);
        void SetEmail(string s);

        string GetUsername();
        string GetPassword();
        string GetEmail();
};

#endif // USER_H
