#ifndef DB_H
#define DB_H

#include <QtSQL>
#include <QDebug>
#include <filesystem>
#include <string>
#include <vector>

#include "user.h"
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


private:
    QSqlDatabase db;

    //initialize database
    std::string dbPath();
    void createTables();

    bool credIsUnique(const QString& credential_type, const QString& credential);


};

#endif // DB_H
