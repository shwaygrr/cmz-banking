#include "system.h"

System::System() : user(nullptr), db(new DB()) {}

System::~System() {
    if (user != nullptr) {
        delete user;
        user = nullptr;
    }
    if (db != nullptr) {
        delete db;
        db = nullptr;
    }
}

void System::setUser(User* current_user) { user = current_user; }

User* System::getUser() const {
    return user;
}

bool System::createAccount(const QString& account_type, const float account_balance) const {
    BankAccount account(user->getUserId(), account_type, account_balance);
    return db->createBankAccount(account);
}

bool System::deleteAccount(const QString& account_number) const {
    if (db->deleteBankAccountByNumber(account_number)){
        db->createActivity(Activity(user->getUserId(), "Account with account #: " + account_number + " deleted"));
        return true;
    } else {
        db->createActivity(Activity(user->getUserId(), "Failed to open bank account"));
        return false;
    }
}

bool System::login(const QString& username_, const QString& password_) {
    if(db->authenticate(username_, password_)) {
        setUser(db->getUserByUsername(username_));
        fetchUserData(user->getUserId());
        return true;
    } else {
        return false;
    }
}

bool System::createUser(const QString& full_name_, const QString& username_, const QString& password_) const {
    return db->createUser(full_name_, username_, password_);
}

void System::fetchUserData(const int id) {
    user->setBankAccounts(db->getAllBankAccountsByUserId(id));
    user->setActivityLog(db->getActivitiesByUserId(id));
}


bool System::createTransaction(const Transaction& transaction) const {
    if (db->createTransaction(transaction)) {
        db->createActivity(Activity(user->getUserId(), transaction));
        return true;
    } else {
        return false;
    }
}

bool System::createActivity(const Activity &activity) const {
    return db->createActivity(activity);
}

bool System::deleteUser() {
    return db->deleteUserById(user->getUserId());
    delete user;
    user = nullptr;
}

bool System::updateUser(const QString& field, const QString& new_value) const {
    return db->updateUserById(user->getUserId(), field, new_value);
}


