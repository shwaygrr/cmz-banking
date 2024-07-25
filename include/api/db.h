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

#include "crypto/hash.h"

class DB {
public:
    DB();
    ~DB();

    //sign up
    void signUp();

    //auth
    void createUser(const QString& full_name, const QString& username, const QString& password_hash);
    User getUserById(const int user_id); //get user data by id
    bool authenticate(const QString& username, const QString& password);

    //user info
    QList<BankAccount> getAllBankAccountsByUserId(const int user_id);
    std::vector<Transaction> getTransactionsById(const int user_id);
    User* getUserByUsername(const QString& username);


    //create
    bool createBankAccount(const BankAccount& new_bank_account);
    void createTransaction(Transaction new_transaction);

    //delete
    bool deleteBankAccountByNumber(const QString& account_number);
private:
    QSqlDatabase db;

    //initialize database
    std::string dbPath();
    void createTables();

    bool credIsUnique(const QString& credential_type, const QString& credential);
};

#endif // DB_H
