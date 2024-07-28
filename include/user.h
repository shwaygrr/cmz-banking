#ifndef USER_H
#define USER_H

#include <QList>
#include <QDebug>
#include <vector>
#include "widgets/BankWidget.h"
#include "bankaccount.h"
#include "activity.h"

class User {
    private:
        int user_id;
        QString full_name, username, created_at;
        QList<Activity> activity_log;
        QList<BankAccount> bank_accounts;
        std::vector<BankWidget*> widgets;

    public:
        User();
        User(int user_id_, QString full_name_, QString username_, QString created_at_);

        //ostream overload
        friend QDebug operator << (QDebug dbg, const User& user);

        //setters
        void setUsername(const QString& username_);
        void setFullName(const QString& full_name_);
        void setActivityLog(const QList<Activity>& activity_log_);
        void setBankAccounts(QList<BankAccount> bank_accounts_);

        //getters
        int getUserId() const;
        QString getFullName() const;
        QString getUsername() const;
        QString getCreatedAt() const;
        QList<Activity> getActivityLog() const;
        QList<BankAccount> getBankAccounts() const;

        //others
        BankAccount* findBankAccount(const QString& account_number);
        void addBankWidget(BankWidget *widget);



        //actions
        void transferMoney(float amount, BankAccount &sender, BankAccount &receiver);
        void sendMoney(float amount, BankAccount sendAccount, User receiver);
        bool createBankAccount(const QString& type, const float balance = 0);
        bool deleteBankAccount(const QString& account_number);
};

#endif // USER_H
