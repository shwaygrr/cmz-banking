#ifndef USER_H
#define USER_H

#include "BankWidget.h"
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
        vector<BankWidget*> widgets;

    public:
        User();
        User(string u, string p, string e);
        User(string u, string p, string e, vector<string> l, vector<BankAccount> a);

        void setUsername(string s);
        void setPassword(string s);
        void setEmail(string s);
        void addToActivityLog(string s);

        BankAccount* findBankAccount(int num);

        void addBankWidget(BankWidget *widget);

        void setActivityLog(vector<string> s);
        void setBankAccounts(vector<BankAccount> a);

        void createBankAccount(int accNum, string type, float balance=0);
        void deleteBankAccount(int accNum);
        void transferMoney(float amount, BankAccount &sender, BankAccount &receiver);
        void sendMoney(float amount, BankAccount sendAccount, User receiver);

        string getUsername();
        string getPassword();
        string getEmail();

        vector<string> getActivityLog();
        vector<BankAccount> getBankAccounts();
};

#endif // USER_H
