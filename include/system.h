#ifndef SYSTEM_H
#define SYSTEM_H

#include "user.h"
#include "api/db.h"
#include "crypto/digsig.h"

class System {
public:
    System();
    ~System();

    bool createAccount(const QString& account_type, const float account_balance);
    bool deleteAccount(const QString& account_number) const;

    bool login(const QString& username_, const QString& password_, const QString& key);
    bool login(const QString& username_, const QString& password_);
    bool createUser(const QString& full_name_, const QString& username_, const QString& password_) const;

    void fetchUserData(const int id);

    void setUser(User* current_user);
    User* getUser() const;

    bool deleteUser();
    bool updateUser(const QString& field, const QString& new_value, const QString& key) const;

    bool createTransaction(const Transaction& transaction) const;
    void createActivity(const Activity& activity) const;

    // QString digitalSignature(const QString& message);

private:
    DB* db;
    User* user;
    Hash* hash;
    DigSig* dig_sig;
    QString generateSalt(const int length = 16);
    void sign(Activity& activity) const;
};

#endif // SYSTEM_H
