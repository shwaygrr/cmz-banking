#include "mainwindow.h"
#include <QApplication>
#include <api/db.h>
#include "bankaccount.h"
using namespace std;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    DB db;

    // User* user = db.getUserByUsername("Ben");

    // BankAccount bank(1 ,"Checking", 10.29);
    // QList<BankAccount> userAccounts = db.getAllBankAccountsByUserId(1);

    // for(const BankAccount& account : userAccounts) {
    //     qDebug() << account;
    //     qDebug() << "-------------------------------------------------";
    // }
    // db.createBankAccount(bank);
    // qDebug() << db.getAllBankAccountsByUserId(4);
    MainWindow w;

    w.show();

    return a.exec();
}
