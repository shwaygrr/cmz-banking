#ifndef USER_H
#define USER_H

#include "widgets/BankWidget.h"
#include "bankaccount.h"
#include <iostream>
#include <string>
#include <vector>
#include <QList>

#include <QDebug>
using namespace std;

class User {
    private:
        int user_id;
        QString full_name, username, created_at;
        vector<string> activityLog;
        QList<BankAccount> bank_accounts;
        vector<BankWidget*> widgets;

    public:
        User();
        User(int user_id_, QString full_name_, QString username_, QString created_at_);

        //ostream overload
        friend QDebug operator << (QDebug dbg, const User& user);

        //setters
        void setUsername(const QString& username_);
        void setFullName(const QString& full_name_);
        void setActivityLog(vector<string> s);
        void setBankAccounts(QList<BankAccount> bank_accounts_);

        //getters
        int getUserId() const;
        QString getFullName() const;
        QString getUsername() const;
        QString getCreatedAt() const;
        vector<string> getActivityLog();
        QList<BankAccount> getBankAccounts();

        //others
        BankAccount* findBankAccount(const QString& account_number);
        void addBankWidget(BankWidget *widget);



        //actions
        void addToActivityLog(string s);
        void transferMoney(float amount, BankAccount &sender, BankAccount &receiver);
        void sendMoney(float amount, BankAccount sendAccount, User receiver);
        bool createBankAccount(const QString& type, const float balance = 0);
        bool deleteBankAccount(const QString& account_number);
};

#endif // USER_H
