#ifndef DB_H
#define DB_H

#include <QtSQL>
#include <QDebug>
#include <filesystem>
#include <string>
#include <vector>

#include "user.h"
#include "bankaccount.h"
#include "transaction.h"
#include "activity.h"
#include "crypto/hash.h"

class DB {
public:
    DB();
    ~DB();

    //sign up
    void signUp();

    //auth
    bool createUser(const QString& full_name, const QString& username, const QString& password_hash);
    User getUserById(const int user_id);
    bool authenticate(const QString& username, const QString& password);

    //user info
    QList<BankAccount> getAllBankAccountsByUserId(const int user_id);
    std::vector<Transaction> getTransactionsByUserId(const int user_id);
    User* getUserByUsername(const QString& username);
    QList<Activity> getActivitiesByUserId(const int user_id);

    //get
    BankAccount getBankAccountById(const int account_id);
    Transaction getTransactionById(const int transaction_id);

    //update
    bool updateUserById(const int id, const QString &field, const QString &new_data);
    bool updateBankAccountBalance(const int account_id, const float new_balance);

    //create
    bool createBankAccount(const BankAccount& new_bank_account);
    bool createTransaction(const Transaction& new_transaction);
    bool createActivity(const Activity& new_activity);

    //delete
    bool deleteBankAccountByNumber(const QString& account_number);
    bool deleteUserById(const int user_id);

private:
    QSqlDatabase db;

    //initialize database
    std::string dbPath();
    bool checkAllTablesExist(const QStringList& tables);
    void createTables();
    void createTriggers();
    bool credIsUnique(const QString& credential_type, const QString& credential);
};

#endif // DB_H
