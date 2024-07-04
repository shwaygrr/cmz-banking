#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>

using namespace std;

//this is where you put helper function declarations

class User{
    private:
        string username, password;
    public:
        User(string u, string p);

        void SetUsername(string s);
        void SetPassword(string s);

        string GetUsername();
        string GetPassword();
};

#endif // USER_H
