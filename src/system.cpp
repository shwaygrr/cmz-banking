#include "system.h"


//set up/make actiity
//create activity
        //sign by refernce
        //send to db

System::System() : user(new User()), db(new DB()), hash(new Hash()), dig_sig(new DigSig()) {}

System::~System() {
        delete user;
        user = nullptr;

        delete db;
        db = nullptr;

        delete dig_sig;
        dig_sig = nullptr;
}

void System::setUser(User* current_user) { user = current_user; }

User* System::getUser() const {
    return user;
}

bool System::createAccount(const QString& account_type, const float account_balance) {
    BankAccount account(user->getUserId(), account_type, account_balance);
    QString activity_description = "Added a new " + account.getAccountType() + " account with an initial balance of $" + QString::number(account.getBalance());
    createActivity(Activity(account.getAccountUserId(), activity_description));
    return db->createBankAccount(account);
}

bool System::deleteAccount(const QString& account_number) const {
    if (db->deleteBankAccountByNumber(account_number)){
        createActivity(Activity(user->getUserId(), "Account with account #: " + account_number + " deleted"));
        return true;
    } else {
        createActivity(Activity(user->getUserId(), "**Failed to delete bank account**"));
        return false;
    }
}

bool System::login(const QString& username_, const QString& password_, const QString& key) {
    if(db->authenticate(username_, password_, key)) {
        setUser(db->getUserByUsername(username_));
        user->setPrivateKey(key);
        fetchUserData(user->getUserId());
        dig_sig->setRSA(user->user_rsa);
        createActivity(Activity(user->getUserId(), "Successful login"));
        return true;
    } else {
        // login unsuccessful not showing in activity log
        createActivity(Activity(user->getUserId(), "Unsuccessful login"));
        return false;
    }
}

bool System::login(const QString& username_, const QString& password_) {
    if(db->authenticate(username_, password_)) {
        QString d_private = user->getPrivateKey();
        setUser(db->getUserByUsername(username_));
        user->setPrivateKey(d_private);
        dig_sig->setRSA(user->user_rsa);
        createActivity(Activity(user->getUserId(), "Successful login"));
        // fetchUserData(user->getUserId());
        return true;
    } else {
        createActivity(Activity(user->getUserId(), "Unsuccessful login"));
        return false;
    }
}

bool System::createUser(const QString& full_name_, const QString& username_, const QString& password_) const {
    user->user_rsa.keyGen(20);
    QString e_public = QString::fromStdString(user->user_rsa.e_public.as_str());
    QString n_public = QString::fromStdString(user->user_rsa.n_public.as_str());

    return db->createUser(full_name_, username_, password_, e_public, n_public);
}

void System::fetchUserData(const int id) {
    user->setBankAccounts(db->getAllBankAccountsByUserId(id));
    user->setActivityLog(db->getActivitiesByUserId(id));

    for (Activity& activity: user->getActivityLog()) {
        if (dig_sig->verify(activity.getDescription().toStdString(), bigint(activity.getSignature().toStdString()))) {
            activity.setValidated(false);
            qDebug() << "Activity:" << activity.getActivityId() << "not validated";
        } else {
            activity.setValidated(true);
            qDebug() << "Activity:" << activity.getActivityId() << "validated";
        }
    }
}


bool System::createTransaction(const Transaction& transaction) const {
    Transaction new_transaction = transaction;
    // add conditional for tpe if sending feature is complete
    new_transaction.setTransactionType("transfer");
    if (db->createTransaction(new_transaction)) {
        createActivity(Activity(user->getUserId(), new_transaction));
        return true;
    } else {
        return false;
    }
}

bool System::deleteUser() {
    return db->deleteUserById(user->getUserId());
    delete user;
    user = nullptr;
}

bool System::updateUser(const QString& field, const QString& new_value, const QString& key) const {
    dig_sig->printRSA();
    //remove key param
    QString field_name = field == "password_hash" ? "Password" : field == "username" ? "Username" : "Full Name";
    if(db->updateUserById(user->getUserId(), field, new_value)) {
        createActivity(Activity(user->getUserId(), field_name + " Updated"));
        return true;
    } else {
        createActivity(Activity(user->getUserId(), "** Failed to update" + field_name + "**"));
        return false;
    }
}


void System::sign(Activity& activity) const {
    if(user->getUserId() == 0 || activity.getUserId() ==  0) {
        qDebug() << "Not authorized";
    }
    qDebug() << "signing";
    QString description = activity.getDescription();
    // dig_sig->printRSA();
    activity.setSignature(QString::fromStdString(dig_sig->sign(description.toStdString())));
    qDebug() << activity.getSignature();
}

void System::createActivity(const Activity& activity) const {
    Activity new_activity = activity;
    sign(new_activity);
    qDebug() << new_activity;
    db->createActivity(new_activity);
}

// QString verifydig_sig->sign(const QString& message) {
//     if(user.id == 0) {
//         qDebug() << "Unauthorized";
//         return;
//     }

// }
