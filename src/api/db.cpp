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

// Destructor to close the database if needed
DB::~DB() {
    if (db.isOpen()) {
        db.close();
    }
}

void DB::createTables() {
    QSqlQuery qry;

    QString createUsersTable = R"(
        CREATE TABLE Users (
            user_id INTEGER PRIMARY KEY AUTOINCREMENT,
            username VARCHAR(255) UNIQUE NOT NULL,
            password_hash VARCHAR(255) NOT NULL,
            full_name VARCHAR(255),
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        );
    )";

    if (!qry.exec(createUsersTable)) {
        qDebug() << "Error creating Users table:" << qry.lastError();
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

    if (!qry.exec(createBankAccountsTable)) {
        qDebug() << "Error creating BankAccounts table:" << qry.lastError();
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

    if (!qry.exec(createTransactionsTable)) {
        qDebug() << "Error creating Transactions table:" << qry.lastError();
    }

    // db.close();
}
