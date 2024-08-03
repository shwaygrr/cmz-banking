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
    user_rsa = RSA();
    user_aes = AES128();
}

User::User(const int user_id_, const QString& full_name_, const QString& username_, const QString& created_at_, const QString& e_public, const QString& n_public, const QString &private_key) {
    user_id = user_id_;
    full_name = full_name_;
    username = username_;
    created_at = created_at_;
    activity_log = {};
    bank_accounts = {};
    user_rsa.setPublicKeys(bigint(e_public.toStdString()), bigint(n_public.toStdString()));
    user_aes.setPrivateKey(private_key.toStdString());
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

QString User::getPrivateKey() const {
    qDebug() << "hi";
    return QString::fromStdString(user_rsa.getPrivateKey().as_str());
}

QList<BankAccount> User::getBankAccounts() const { return bank_accounts; }

void User::setActivityLog(const QList<Activity>& activity_log_) { activity_log = activity_log_; }

void User::setBankAccounts(QList<BankAccount> bank_accounts_) { bank_accounts = bank_accounts_; }

BankAccount* User::findBankAccount(const QString& account_number) {
    for (BankAccount& bank_account : bank_accounts) {
        if (bank_account.getAccountNumber() == account_number) {
            return &bank_account;
        }
    }
    qDebug() << "Couldn't find account";
    return nullptr;
}

void User::setPrivateKey(const QString& key) {
    user_rsa.setPrivateKey(bigint(key.toStdString()));
}
