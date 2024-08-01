#include "mainwindow.h"
#include <QApplication>
#include <api/db.h>
#include "bankaccount.h"
#include "transaction.h"
#include "activity.h"
#include "system.h"

#include "crypto/rsa.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    // RSA rsa;
    // rsa.keyGen(10);

    // rsa.verifyPrivateKey(rsa.getPrivateKey(), rsa.e_public, rsa.n_public);
    // AES128 aes;
    // qDebug() << aes.generatePrivateKey();
    // RSA rsa;

    // bigint b = rsa.encrypt(123);
    // std::cout << rsa.decrypt(a);



    // rsa.rsaEnc(123);
  //   bigint j("1232366543"), s("3454"), y("223"), odd_num("334556");

  //   if (j >= s - 1 && y != odd_num - 1)
  //       qDebug() << "hi";


    // DB db;

    // QList<Activity> activities = db.getActivitiesByUserId(7);

    // for(const Activity& activity : activities) {
    //     qDebug() << activity;
    //     qDebug() << "------------------------------------------";
    // }
    // Transaction transaction("gas money", 1, 2, 10.00);
    // transaction.setTransactionType("transfer");
    // Activity activity(1, transaction);
    // // qDebug() << activity;

    // db.createActivity(t);
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
    return 0;
}
