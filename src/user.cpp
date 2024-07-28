#include "user.h"
#include "widgets/BankWidget.h"
#include "transaction.h"
#include <vector>
#include <QDebug>
#include <string>
#include "api/db.h"

User::User() {
    user_id = 0;
    full_name = "";
    username = "";
    created_at = "";
}

User::User(int user_id_, QString full_name_, QString username_, QString created_at_) {
    user_id = user_id_;
    full_name = full_name_;
    username = username_;
    created_at = created_at_;
    activity_log = {};
    bank_accounts = {};
}

QDebug operator << (QDebug dbg, const User& user) {
    QDebugStateSaver saver(dbg);
    dbg.nospace();

    if (user.getUserId() == 0) {
        dbg << "Invalid User -- User undefined";
    } else {
        dbg << "User ID: " << user.getUserId() << "\n"
            << "Full Name: " << user.getFullName() << "\n"
            << "Username: " << user.getUsername() << "\n"
            << "Created At: " << user.getCreatedAt();
    }
    return dbg;
}


//add security levels and error handling to getters later
int User::getUserId() const { return user_id; }

QString User::getFullName() const { return full_name; }

QString User::getUsername() const { return username; }

QString User::getCreatedAt() const { return created_at; }

QList<Activity> User::getActivityLog() const { return activity_log; }

QList<BankAccount> User::getBankAccounts() const { return bank_accounts; }

void User::setActivityLog(const QList<Activity>& activity_log_) { activity_log = activity_log_; }

void User::setBankAccounts(QList<BankAccount> bank_accounts_) { bank_accounts = bank_accounts_; }

bool User::createBankAccount(const QString& account_type, const float balance) {
    DB db;
    BankAccount account(user_id, account_type, balance);
    return db.createBankAccount(account);
}

bool User::deleteBankAccount(const QString& account_number) {
    DB db;
    if(db.deleteBankAccountByNumber(account_number)){
        db.createActivity(Activity(user_id, "Account with account #: " + account_number + " deleted"));
        return true;
    } else {
        db.createActivity(Activity(user_id, "Failed to open bank account"));
        return false;
    }
}

BankAccount* User::findBankAccount(const QString& account_number) {
    for (BankAccount& bank_account : bank_accounts) {
        if (bank_account.getAccountNumber() == account_number) {
            return &bank_account;
        }
    }

    qDebug() << "Couldn't find account";
    return nullptr;
}

