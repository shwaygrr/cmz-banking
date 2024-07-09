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

        void SetUsername(string s);
        void SetPassword(string s);
        void SetEmail(string s);
        void AddToActivityLog(string s);
        void CreateBankAccount(int accNum, string type, float balance=0);
        void TransferMoney(float amount, BankAccount sender, BankAccount receiver);
        void SendMoney(float amount, BankAccount sendAccount, User receiver);

        string GetUsername();
        string GetPassword();
        string GetEmail();

        vector<string> GetActivityLog();
};

#endif // USER_H
