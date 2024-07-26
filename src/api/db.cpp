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
    createTriggers();
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

void DB::createTriggers() {
    QSqlQuery query;

    QString createBankAccountsTrigger = R"(
        CREATE TRIGGER IF NOT EXISTS update_bankaccounts_updated_at
        AFTER UPDATE ON BankAccounts
        FOR EACH ROW
        BEGIN
            UPDATE BankAccounts
            SET updated_at = CURRENT_TIMESTAMP
            WHERE account_id = OLD.account_id;
        END;
    )";

    if (!query.exec(createBankAccountsTrigger)) {
        qDebug() << "Failed to create trigger for BankAccounts:" << query.lastError().text();
    }

    // Trigger for Users table
    QString createUsersTrigger = R"(
        CREATE TRIGGER IF NOT EXISTS update_users_updated_at
        AFTER UPDATE ON Users
        FOR EACH ROW
        BEGIN
            UPDATE Users
            SET updated_at = CURRENT_TIMESTAMP
            WHERE user_id = OLD.user_id;
        END;
    )";

    if (!query.exec(createUsersTrigger)) {
        qDebug() << "Failed to create trigger for Users:" << query.lastError().text();
    }
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


bool DB::updateUserById(const int id, const QString &field, const QString &new_data) {
    QSqlQuery query;
    QString queryString;

    if (field == "full_name" || field == "username" || field == "password_hash") {
        queryString = "UPDATE Users SET " + field + " = :new_data WHERE user_id = :id";
    } else {
        qDebug() << "Invalid field name:" << field;
        return false;
    }

    query.prepare(queryString);
    query.bindValue(":new_data", new_data);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error updating profile:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() > 0) {
        qDebug() << "Profile updated successfully.";
        return true;
    } else {
        qDebug() << "Profile with ID" << id << "not found or no changes made.";
        return false;
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

User* DB::getUserByUsername(const QString& username) {
    QSqlQuery query;

    query.prepare("SELECT user_id, full_name, username, password_hash, created_at FROM Users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Error retrieving user:" << query.lastError().text();
        return nullptr;
    }

    if (query.next()) {
        QSqlRecord record = query.record();

        int user_id = record.value("user_id").toInt();
        QString full_name = record.value("full_name").toString();
        QString username = record.value("username").toString();
        QString created_at = record.value("created_at").toDateTime().toString();


        return new User(user_id, full_name, username, created_at);
    } else {
        qDebug() << "User not found";
        return nullptr;
    }
}


QList<BankAccount> DB::getAllBankAccountsByUserId(const int user_id) {
    QList<BankAccount> accounts;

    QSqlQuery query;
    query.prepare("SELECT account_id, user_id, account_number, account_type, balance, created_at, updated_at FROM BankAccounts WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qDebug() << "Error retrieving bank accounts:" << query.lastError().text();
        return accounts;
    }

    while (query.next()) {
        QSqlRecord record = query.record();

        int account_id = record.value("account_id").toInt();
        int user_id = record.value("user_id").toInt();
        QString account_number = record.value("account_number").toString();
        QString account_type = record.value("account_type").toString();
        float balance = record.value("balance").toFloat();
        QString created_at = record.value("created_at").toString();

        accounts.append(BankAccount(account_id, user_id, account_number, account_type, balance, created_at));
    }

    return accounts;
}


bool DB::createBankAccount(const BankAccount& new_bank_account) {
    QSqlQuery query;

    if(new_bank_account.getAccountUserId() == 0) {
        qDebug() << "Error creating bank account: User id is required";
        return false;
    }

    if(new_bank_account.getAccountType() != "Checking" && new_bank_account.getAccountType() != "Saving") {
        qDebug() << "Error creating bank account: Invalid account type";
        return false;
    }

    query.prepare("INSERT INTO BankAccounts (user_id, account_number, account_type, balance, created_at, updated_at) "
                  "VALUES (:user_id, :account_number, :account_type, :balance, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)");
    query.bindValue(":user_id", new_bank_account.getAccountUserId());
    query.bindValue(":account_number", new_bank_account.getAccountNumber());
    query.bindValue(":account_type", new_bank_account.getAccountType());
    query.bindValue(":balance", new_bank_account.getBalance());

    if (!query.exec()) {
        qDebug() << "Error inserting new bank account:" << query.lastError().text();
        return false;
    } else {
        qDebug() << "New bank account created successfully.";
        return true;
    }
}


bool DB::deleteBankAccountByNumber(const QString& account_number) {
    QSqlQuery query;

    query.prepare("DELETE FROM BankAccounts WHERE account_number = :account_number");
    query.bindValue(":account_number", account_number);

    if (!query.exec()) {
        qDebug() << "Error deleting bank account:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() > 0) {
        qDebug() << "Bank account deleted successfully.";
        return true;
    } else {
        qDebug() << "Bank account with ID" << account_number << "not found.";
        return false;
    }
}
