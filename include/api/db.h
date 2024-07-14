#ifndef DB_H
#define DB_H

#include <QtSQL>
#include <QDebug>
#include <filesystem>
#include <string>
#include <vector>
class DB {
public:
    DB();
    ~DB();
    //sign in

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

    std::string dbPath();
    void createTables();
};

#endif // DB_H
