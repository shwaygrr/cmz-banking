#include "mainwindow.h"
#include <QApplication>
#include <api/db.h>
#include "bankaccount.h"
#include "transaction.h"
using namespace std;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    DB db;

    // Transaction transaction("save some money", 1, 2, 2.00);

    // db.createTransaction(transaction);

    // Transaction  transaction_from_db = db.getTransactionById(4);
    // qDebug() << transaction_from_db;
    // BankAccount account = db.getBankAccountById(2);
    // qDebug() << account;

    // db.updateUserById(3, "password_hash", "1234");
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
