#ifndef SYSTEM_H
#define SYSTEM_H

#include "user.h"
#include "mainwindow.h"

class System : MainWindow {
public:
    System();
    ~System();
private:
    DB db;

    User user = null;

    void login();
    void logout();
    void createUser();

    void loadAllAccounts();
    void loadAccount();
    void deleteAccounts();


    void saveUser();
};

#endif // SYSTEM_H
