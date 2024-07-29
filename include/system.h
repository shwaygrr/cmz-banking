#ifndef SYSTEM_H
#define SYSTEM_H

#include "user.h"
#include "api/db.h"

class System {
public:
    System();
    ~System();

    bool createAccount(const QString& account_type, const float account_balance) const;
    bool deleteAccount(const QString& account_number) const;

    bool login(const QString& username_, const QString& password_);
    bool createUser(const QString& full_name_, const QString& username_, const QString& password_) const;

    void fetchUserData(const int id);

    void setUser(User* current_user);
    User* getUser() const;

    bool deleteUser();
    bool updateUser(const QString& field, const QString& new_value) const;

    bool createTransaction(const Transaction& transaction) const;
    bool createActivity(const Activity &activity) const;

private:
    DB* db;
    User* user;

    QString generateSalt(const int length = 16);
};

#endif // SYSTEM_H
