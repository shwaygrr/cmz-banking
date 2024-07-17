#ifndef DB_H
#define DB_H

#include <QtSQL>
#include <QDebug>
#include <filesystem>
#include <string>
#include <vector>

#include "user.h"
class DB {
public:
    DB();
    ~DB();
    //user table
    bool createUser(const QString& full_name, const QString& username, const QString& password_hash); //create user
    User getUserById(const int user_id); //get user data by id

    //sign up
    void signUp();

    //sign out

    //create bank account
    //delete bank account

    //transfer money
    //send money

    //bank log


private:
    QSqlDatabase db;

    //initialize database
    std::string dbPath();
    void createTables();
    //user table operations
    //create user

    //get user


};

#endif // DB_H
