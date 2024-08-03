#ifndef USER_H
#define USER_H

#include <QList>
#include <QDebug>
#include <vector>
#include "widgets/BankWidget.h"
#include "bankaccount.h"
#include "activity.h"
#include "crypto/aes.h"
#include "crypto/rsa.h"

class User {
    private:
        int user_id;
        QString full_name, username, created_at;
        QList<Activity> activity_log;
        QList<BankAccount> bank_accounts;
        std::vector<BankWidget*> widgets;

    public:
        User();
        User(const int user_id_, const QString& full_name_, const QString& username_, const QString& created_at_, const QString& e_public, const QString& n_public, const QString& private_key);

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
        QString getPrivateKey() const;

        QList<Activity> getActivityLog() const;
        QList<BankAccount> getBankAccounts() const;

        void setPrivateKey(const QString& key);
        //others
        BankAccount* findBankAccount(const QString& account_number);
        void addBankWidget(BankWidget *widget);

        RSA user_rsa;
        AES128 user_aes;
};

#endif // USER_H
