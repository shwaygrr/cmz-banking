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

    const QStringList tables = {"Users", "BankAccounts", "Transactions", "Activities"};

    if(!checkAllTablesExist(tables)) createTables();

    createTriggers();
}


DB::~DB() {
    if (db.isOpen()) {
        db.close();
    }
}

bool DB::checkAllTablesExist(const QStringList& tables) {
    QSqlQuery query;

    for (const QString& table : tables) {
        query.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name=:tableName;");
        query.bindValue(":tableName", table);

        if (!query.exec()) {
            qDebug() << "Error executing query:" << query.lastError();
            return false;
        }

        // If the table is not found
        if (!query.next()) {
            qDebug() << table << " does not exist:" << table;
            return false;
        }
    }
    return true;
}

void DB::createTables() {
    QSqlQuery query;

    QString createUsersTable = R"(
        CREATE TABLE Users (
            user_id INTEGER PRIMARY KEY AUTOINCREMENT,
            full_name VARCHAR(255),
            username VARCHAR(255) UNIQUE NOT NULL,
            password_hash VARCHAR(255) NOT NULL,
            salt VARCHAR(255) NOT NULL,
            e_public VARCHAR(255) NOT NULL,
            n_public VARCHAR(255) NOT NULL,
            private_key_enc VARCHAR(255) NOT NULL,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        );
    )";

    if (!query.exec(createUsersTable))
        qDebug() << "Error creating Users table:" << query.lastError();

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

    if (!query.exec(createBankAccountsTable))
        qDebug() << "Error creating BankAccounts table:" << query.lastError();

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

    if (!query.exec(createTransactionsTable))
        qDebug() << "Error creating Transactions table:" << query.lastError();

    QString createActivitiesTable = R"(
        CREATE TABLE Activities (
            activity_id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER,
            description VARCHAR(255),
            signature VARCHAR(255),
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (user_id) REFERENCES Users(user_id)
        );
    )";

    if (!query.exec(createActivitiesTable))
        qDebug() << "Error creating Activities table:" << query.lastError();
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

User DB::getUserById(const int user_id, const QString& key) {
    QSqlQuery query;
    RSA rsa;
    // Prepare the SQL query to select a user by ID
    query.prepare("SELECT user_id, full_name, username, password_hash, e_public, n_public, private_key_enc, created_at FROM Users WHERE user_id = :user_id");
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
        QString e_public = record.value("e_public").toString();
        QString n_public = record.value("n_public").toString();
        QString private_key_enc = record.value("private_key_enc").toString();

        //decrypt private key
        qDebug() << "Private key enc:" << private_key_enc;

        // QString private_key = QString::fromStdString(rsa.decrypt(bigint(private_key_enc.toStdString()), bigint(e_public.toStdString()), bigint(key.toStdString())));

        // qDebug() << "private key: " << private_key;
        return User(user_id, full_name, username, created_at, e_public, n_public, private_key_enc);
        return User();
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


bool DB::createUser(const QString& full_name, const QString& username, const QString& password, const QString& e_public, const QString& n_public) {
    QSqlQuery query;
    Hash hash;
    RSA rsa;
    AES128 aes;

    std::string salt = generateSalt();

    bigint private_key = binToInt<128>(hexToBin<128>(aes.generatePrivateKey()));

    QString private_key_enc = QString::fromStdString(rsa.encrypt(private_key, e_public.toStdString(), n_public.toStdString()).as_str());


    if (credIsUnique("username", username)) {
        query.prepare("INSERT INTO Users (full_name, username, password_hash, salt, e_public, n_public, private_key_enc) VALUES (:full_name, :username, :password_hash, :salt, :e_public, :n_public, :private_key_enc)");
        query.bindValue(":full_name", full_name);
        query.bindValue(":username", username);
        query.bindValue(":password_hash", QString::fromStdString(hash.hash(password.toStdString(), salt)));
        query.bindValue(":salt", QString::fromStdString(salt));
        query.bindValue(":e_public", e_public);
        query.bindValue(":n_public", n_public);
        query.bindValue(":private_key_enc", private_key_enc);

        if (!query.exec()) {
            qDebug() << "Error adding user:" << query.lastError().text();
            return false;
        }
        qDebug() << "User added successfully";
        return true;
    } else {
        qDebug() << "Username already exists";
        return false;
    }
}



bool DB::updateUserById(const int id, const QString &field, const QString &new_data) {
    QSqlQuery query;
    QString queryString;

    // Validate field name
    if (field == "full_name" || field == "username" || field == "password_hash") {
        queryString = "UPDATE Users SET " + field + " = :new_data, updated_at = CURRENT_TIMESTAMP WHERE user_id = :id";
    } else {
        qDebug() << "Invalid field name:" << field;
        return false;
    }

    query.prepare(queryString);
    query.bindValue(":new_data", new_data);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
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


bool DB::deleteUserById(const int user_id) {
    QSqlQuery query;

    query.prepare("DELETE FROM Users WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qDebug() << "Error deleting user:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0) {
        qDebug() << "No user found with the provided ID.";
        return false;
    }

    qDebug() << "User with ID" << user_id << "was successfully deleted.";
    return true;
}



bool DB::authenticate(const QString& username, const QString& password, const QString& key) {
    QSqlQuery query;
    Hash hash;
    query.prepare("SELECT user_id, salt, password_hash, e_public, n_public FROM Users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Query Error" << query.lastError().text();
        return false;
    }

    //check username
    if (query.next()) {
        QSqlRecord record = query.record();
        QString password_hash = record.value("password_hash").toString();
        bigint e_public(record.value("e_public").toString().toStdString());
        bigint n_public(record.value("n_public").toString().toStdString());

        std::string salt = record.value("salt").toString().toStdString();
        // int user_id = record.value("user_id").toInt();

        //check password
        if (password_hash.toStdString() == hash.hash(password.toStdString(), salt)) {

            //check key
            RSA rsa;
            if (!rsa.verifyPrivateKey(bigint(key.toStdString()), e_public, n_public)) {
                qDebug() << "Invalid Key";
                return false;
            }

            qDebug() << "Authenticated!";
            return true;
        } else {
            qDebug() << "invalid password";
            // createActivity(Activity(user_id, "Unsuccessful login attempt"));
            return false;
        }
    } else {
        qDebug() << "Invalid username";
        return false;
    }
}


bool DB::authenticate(const QString& username, const QString& password) {
    QSqlQuery query;
    Hash hash;
    query.prepare("SELECT user_id, salt, password_hash FROM Users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Query Error" << query.lastError().text();
        return false;
    }

    //check username
    if (query.next()) {
        QSqlRecord record = query.record();
        QString password_hash = record.value("password_hash").toString();
        std::string salt = record.value("salt").toString().toStdString();
        int user_id = record.value("user_id").toInt();


        qDebug() << "Key validated";
        //check password
        if (password_hash.toStdString() == hash.hash(password.toStdString(), salt)) {
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

    query.prepare("SELECT user_id, full_name, username, password_hash, created_at, e_public, n_public, private_key_enc FROM Users WHERE username = :username");
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
        QString e_public_key = record.value("e_public").toString();
        QString n_public_key = record.value("n_public").toString();
        QString private_key = record.value("private_key").toString();

        // createActivity(Activity(user_id, "Successfully Logged in"));

        return new User(user_id, full_name, username, created_at, e_public_key, n_public_key, private_key);
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

    if(new_bank_account.getAccountType() != "Checking" && new_bank_account.getAccountType() != "Savings") {
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
        qDebug() << "Error Creating new bank account:" << query.lastError().text();
        return false;
    } else {
        // createActivity(Activity(new_bank_account.getAccountUserId(), "Added a new " + new_bank_account.getAccountType() + " account with an initial balance of $" + QString::number(new_bank_account.getBalance())));
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

BankAccount DB::getBankAccountById(const int account_id) {
    QSqlQuery query;
    query.prepare("SELECT * FROM BankAccounts WHERE account_id = :account_id");
    query.bindValue(":account_id", account_id);

    if (!query.exec()) {
        qDebug() << "Error retrieving bank account:" << query.lastError().text();
        return BankAccount(); // Return a default BankAccount object in case of error
    }

    if (query.next()) {
        QSqlRecord record = query.record();

        int account_id = record.value("account_id").toInt();
        int user_id = record.value("user_id").toInt();
        QString account_number = record.value("account_number").toString();
        QString account_type = record.value("account_type").toString();
        float balance = record.value("balance").toFloat();
        QString created_at = record.value("created_at").toString();

        return BankAccount(account_id, user_id, account_number, account_type, balance, created_at);
    } else {
        qDebug() << "Bank account not found";
        return BankAccount();
    }
}

bool DB::updateBankAccountBalance(const int account_id, const float new_balance) {
    QSqlQuery query;

    query.prepare("UPDATE BankAccounts SET balance = :new_balance, created_at = CURRENT_TIMESTAMP WHERE account_id = :account_id");
    query.bindValue(":new_balance", new_balance);
    query.bindValue(":account_id", account_id);

    if (!query.exec()) {
        qDebug() << "Error updating bank account balance:" << query.lastError().text();
        return false;
    }

    return true;
}


bool DB::createTransaction(const Transaction& new_transaction) {
    QSqlQuery query;

    BankAccount from_account = getBankAccountById(new_transaction.getSenderId());

    float transaction_amount = new_transaction.getAmount();

    if (from_account.getBalance() < transaction_amount) {
        qDebug() << "Insufficient funds in the sender's account.";
        return false;
    }

    BankAccount to_account = getBankAccountById(new_transaction.getReceiverId());

    if (to_account.getAccountId() == 0) {
        qDebug() << "Error: receiver account doesn't exist";
        return false;
    }

    QString transfer_type =  from_account.getAccountUserId() == to_account.getAccountUserId() ? "transfer" : "send";

    if (!updateBankAccountBalance(from_account.getAccountId(), from_account.getBalance() - transaction_amount)) {
        qDebug() << "Error updating sender's account balance.";
        return false;
    }

    if (!updateBankAccountBalance(to_account.getAccountId(), to_account.getBalance() + transaction_amount)) {
        qDebug() << "Error updating receiver's account balance.";
        return false;
    }

    query.prepare(R"(
        INSERT INTO Transactions (sender_account_id, receiver_account_id, transaction_type, amount, description, transaction_date)
        VALUES (:sender_account_id, :receiver_account_id, :transaction_type, :amount, :description, CURRENT_TIMESTAMP)
    )");
    query.bindValue(":sender_account_id", new_transaction.getSenderId());
    query.bindValue(":receiver_account_id", new_transaction.getReceiverId());
    query.bindValue(":transaction_type", transfer_type);
    query.bindValue(":amount", new_transaction.getAmount());
    query.bindValue(":description", new_transaction.getDescription());

    if (!query.exec()) {
        qDebug() << "Error inserting transaction into the database:" << query.lastError().text();
        return false;
    }

    return true;
}


Transaction DB::getTransactionById(const int transaction_id) {
    QSqlQuery query;

    query.prepare(R"(
        SELECT transaction_id, sender_account_id, receiver_account_id, transaction_type, amount, description, transaction_date
        FROM Transactions
        WHERE transaction_id = :transaction_id
    )");

    query.bindValue(":transaction_id", transaction_id);

    if (!query.exec()) {
        qDebug() << "Error retrieving transaction:" << query.lastError().text();
        return Transaction();
    }

    if (query.next()) {
        QSqlRecord record = query.record();

        int transaction_id = record.value("transaction_id").toInt();
        int sender_id = record.value("sender_account_id").toInt();
        int receiver_id = record.value("receiver_account_id").toInt();
        QString transaction_type = record.value("transaction_type").toString();
        float amount = record.value("amount").toFloat();
        QString description = record.value("description").toString();
        QString created_at = record.value("transaction_date").toString();

        return Transaction(transaction_id, transaction_type, description, sender_id, receiver_id, amount, created_at);
    } else {
        qDebug() << "Transaction not found";
        return Transaction();  // Return an empty transaction object if no transaction found
    }
}


bool DB::createActivity(const Activity& new_activity) {
    QSqlQuery query;

    query.prepare(R"(
        INSERT INTO Activities (user_id, description, signature, created_at)
        VALUES (:user_id, :description, :signature, CURRENT_TIMESTAMP)
    )");

    query.bindValue(":user_id", new_activity.getUserId());
    query.bindValue(":description", new_activity.getDescription());
    query.bindValue(":signature", new_activity.getSignature());

    if (!query.exec()) {
        qDebug() << "Error creating activity:" << query.lastError();
        return false;
    }
    qDebug() << "activity created";
    return true;
}


QList<Activity> DB::getActivitiesByUserId(const int user_id) {
    QList<Activity> activities;
    QSqlQuery query;
    query.prepare("SELECT activity_id, user_id, description, signature, created_at FROM Activities WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (!query.exec()) {
        qDebug() << "Error retrieving activities:" << query.lastError();
        return activities;
    }

    while (query.next()) {
        int activity_id = query.value("activity_id").toInt();
        QString description = query.value("description").toString();
        QString created_at = query.value("created_at").toString();
        QString signature = query.value("signature").toString();
        Activity activity(activity_id, user_id, description, signature, created_at);

        activities.append(activity);
    }

    return activities;
}

QSet<QString> DB::getPublicKeysById(int user_id) {
    QSet<QString> publicKeys;
    QSqlQuery query;

    // Prepare the query to fetch e_public and n_public from the Users table
    query.prepare("SELECT e_public, n_public FROM Users WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    // Execute the query
    if (!query.exec()) {
        qDebug() << "Error fetching public keys:" << query.lastError().text();
        return publicKeys;
    }

    // If the query is successful and there is at least one result, add the keys to the set
    if (query.next()) {
        publicKeys.insert(query.value("e_public").toString());
        publicKeys.insert(query.value("n_public").toString());
    }

    return publicKeys;
}
