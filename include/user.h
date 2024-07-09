#ifndef USER_H
#define USER_H

#include "bankaccount.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//this is where you put helper function declarations

class User{
    private:
        string username, password, email;
        vector<string> activityLog;
        vector<BankAccount> accounts;
    public:
        User();
        User(string u, string p, string e);
        User(string u, string p, string e, vector<string> l, vector<BankAccount> a);

        void SetUsername(string s);
        void SetPassword(string s);
        void SetEmail(string s);
        void AddToActivityLog(string s);
        void CreateBankAccount(int accNum, string type, float balance);
        BankAccount FindBankAccount(int num);

        void SetActivityLog(vector<string> s);
        void SetBankAccounts(vector<BankAccount> a);

        string GetUsername();
        string GetPassword();
        string GetEmail();

        vector<string> GetActivityLog();
        vector<BankAccount> GetBankAccounts();
};

#endif // USER_H
