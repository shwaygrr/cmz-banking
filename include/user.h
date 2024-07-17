#ifndef USER_H
#define USER_H

#include "BankWidget.h"
#include "bankaccount.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//this is where you put helper function declarations

class User {
    private:
        QString full_name, username, created_at;
        int user_id;
        vector<string> activityLog;
        vector<BankAccount> accounts;
        vector<BankWidget*> widgets;
    public:
        User();
        User(int user_id_, QString full_name_, QString username_, QString created_at);

        friend QDebug operator << (QDebug dbg, const User& user);

        int getUserId() const;
        QString getFullName() const;
        QString getUsername() const;
        QString getCreatedAt() const;

        BankAccount* findBankAccount(int num);

        void addBankWidget(BankWidget *widget);

        void setActivityLog(vector<string> s);
        void setBankAccounts(vector<BankAccount> a);

        void createBankAccount(int accNum, string type, float balance=0);
        void deleteBankAccount(int accNum);
        void transferMoney(float amount, BankAccount &sender, BankAccount &receiver);
        void sendMoney(float amount, BankAccount sendAccount, User receiver);

        vector<string> getActivityLog();
        vector<BankAccount> getBankAccounts();
};

#endif // USER_H
