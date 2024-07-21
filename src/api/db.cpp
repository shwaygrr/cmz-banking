#include "api/db.h"

namespace fs = std::filesystem;
/*
    Database Route
        - Determines where SQL folder will be stored within project
*/
#include <iostream>
#include <filesystem>
#include <QDebug>
#include <QString>

#include <stdexcept>

namespace fs = std::filesystem;

/********************************************Constructor/Destructor and Associated Helpers********************************************/
std::string DB::dbPath() {
    fs::path currentPath = fs::current_path();

    while (!currentPath.empty() && currentPath.filename().string() != "cmz-banking") {
        currentPath = currentPath.parent_path();
    }

    if (currentPath.empty()) {
        qDebug() << "Project root not found";
        throw std::runtime_error("Project root not found -- Make sure the project name is cmz-banking");
    }

    fs::path dataFolder = currentPath / "data" / "db.sqlite";

    return dataFolder.string();
}


DB::DB() {
    QString dbRoutePath = QString::fromStdString(dbPath());
    // qDebug() << "Database path:" << dbRoutePath;

    try {
        db = QSqlDatabase::addDatabase("QSQLITE");
        qDebug() << "Database added";

        db.setDatabaseName(dbRoutePath);
        qDebug() << "Database name set to:" << dbRoutePath;

        if (!db.open()) {
            qDebug() << "Failed to open the database:" << db.lastError().text();
            throw std::runtime_error("Failed to open the database");
        }
        qDebug() << "Database opened successfully";
    } catch (const std::exception& e) {
        qDebug() << "Error:" << e.what();
    }
    createTables();
}


DB::~DB() {
    if (db.isOpen()) {
        db.close();
    }
}


void DB::createTables() {
    QSqlQuery query;

    QString createUsersTable = R"(
        CREATE TABLE Users (
            user_id INTEGER PRIMARY KEY AUTOINCREMENT,
            full_name VARCHAR(255),
            username VARCHAR(255) UNIQUE NOT NULL,
            password_hash VARCHAR(255) NOT NULL,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        );
    )";

    if (!query.exec(createUsersTable)) {
        qDebug() << "Error creating Users table:" << query.lastError();
    }

    QString createBankAccountsTable = R"(
        CREATE TABLE BankAccounts (
            account_id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER,
            account_number VARCHAR(20) UNIQUE NOT NULL,
            account_type VARCHAR(50),
            balance DECIMAL(15, 2) DEFAULT 0.00,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (user_id) REFERENCES Users(user_id)
        );
    )";

    if (!query.exec(createBankAccountsTable)) {
        qDebug() << "Error creating BankAccounts table:" << query.lastError();
    }

    QString createTransactionsTable = R"(
        CREATE TABLE Transactions (
            transaction_id INTEGER PRIMARY KEY AUTOINCREMENT,
            sender_account_id INTEGER,
            receiver_account_id INTEGER,
            transaction_type VARCHAR(50),
            amount DECIMAL(15, 2),
            transaction_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            description VARCHAR(255),
            FOREIGN KEY (sender_account_id) REFERENCES BankAccounts(account_id),
            FOREIGN KEY (receiver_account_id) REFERENCES BankAccounts(account_id)
        );
    )";

    if (!query.exec(createTransactionsTable)) {
        qDebug() << "Error creating Transactions table:" << query.lastError();
    }

    // db.close();
}


User DB::getUserById(const int user_id) {
    QSqlQuery query;
    // Prepare the SQL query to select a user by ID
    query.prepare("SELECT user_id, full_name, username, password_hash, created_at FROM Users WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    // Execute the query and check for errors
    if (!query.exec()) {
        qDebug() << "Error retrieving user:" << query.lastError().text();
        return User(); // Return an empty user object if there's an error
    }

    // Process the query result
    if (query.next()) {
        QSqlRecord record = query.record();

        int user_id = record.value("user_id").toInt();
        QString full_name = record.value("full_name").toString();
        QString username = record.value("username").toString();
        QString created_at = record.value("created_at").toString();

        return User(user_id, full_name, username, created_at);
    } else {
        qDebug() << "User not found";
        return User();
    }
}
/********************************************Auth functions********************************************/
bool DB::credIsUnique(const QString& credential_type, const QString& credential) {
    QSqlQuery query;

    QString queryString = "SELECT COUNT(*) FROM Users WHERE " + credential_type + " = :credential";

    query.prepare(queryString);
    query.bindValue(":credential", credential);

    if (!query.exec()) {
        qDebug() << "Error checking" << credential_type << ":" << query.lastError().text();
        return false;
    }

    query.next();
    if (query.value(0).toInt() > 0) {
        qDebug() << credential_type << " already exists";
        return false;
    }

    qDebug() << credential_type << " confirmed unique";
    return true;
}


void DB::createUser(const QString& full_name, const QString& username, const QString& password) {
    QSqlQuery query;
    Hash hash;

    if (credIsUnique("username", username)) {
        query.prepare("INSERT INTO Users (full_name, username, password_hash) VALUES (:full_name, :username, :password_hash)");
        query.bindValue(":full_name", full_name);
        query.bindValue(":username", username);
        query.bindValue(":password_hash", QString::fromStdString(hash.hash(password.toStdString())));

        if (!query.exec()) {
            qDebug() << "Error adding user:" << query.lastError().text();
            return;
        }
        qDebug() << "User added successfully";
    } else {
        qDebug() << "Username already exists";
    }
}


bool DB::authenticate(const QString& username, const QString& password) {
    QSqlQuery query;
    Hash hash;
    query.prepare("SELECT password_hash FROM Users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Query Error" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        QSqlRecord record = query.record();
        QString password_hash = record.value("password_hash").toString();

        if (password_hash.toStdString() == hash.hash(password.toStdString())) {
            qDebug() << "Authenticated!";
            return true;
        } else {
            qDebug() << "invalid password";
            return false;
        }
    } else {
        qDebug() << "Invalid username";
        return false;
    }
}

// User DB::getUserById(const int user_id) {
//     QSqlQuery query;

//     // Prepare the SQL query to select a user by ID
//     query.prepare("SELECT user_id, full_name, username, password_hash, created_at FROM Users WHERE user_id = :user_id");
//     query.bindValue(":user_id", user_id);

//     // Execute the query and check for errors
//     if (!query.exec()) {
//         qDebug() << "Error retrieving user:" << query.lastError().text();
//         return User(); // Return an empty user object if there's an error
//     }

//     // Process the query result
//     if (query.next()) {
//         QSqlRecord record = query.record();

//         int user_id = record.value("user_id").toInt();
//         QString full_name = record.value("full_name").toString();
//         QString username = record.value("username").toString();
//         QString created_at = record.value("created_at").toString();

//         return User(user_id, full_name, username, created_at);
//     } else {
//         qDebug() << "User not found";
//         return User();
//     }
// }
