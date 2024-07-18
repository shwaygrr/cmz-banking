#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "BankWidget.h"
#include "ActivityWidget.h"
#include <QMainWindow>
#include <QWidget>
#include <QUiLoader>
#include <QVBoxLayout>
#include <QFile>
#include <QPushButton>

#include <user.h>

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
using namespace std;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    vector<User> users;


private slots:


private:
    Ui::MainWindow *ui;
    QVBoxLayout *layout;
    QWidget *centralWidget;
    QUiLoader loader;

    void loadUI(const QString &uiFile);
    void setupButtonConnections();

    void login();
    void createUser();
    void saveUser();
    void deleteAllAccountsUI();
    void loadAllAccounts();

    BankWidget* createAccount(string accountNumber, string accountType, string accountBalance);
    BankWidget* loadAccount(string accountNumber, string accountType, string accountBalance);
    ActivityWidget* createActivity(string activity, string time);
    void deleteAccount(string accountNumber);

    bool checkForUser(string username);
    bool checkPassword(string password);

    bool verifyAction();

    bool checkForUser(string username);
    bool checkPassword(string password);

    int checkEmail(const string &email);
};
#endif // MAINWINDOW_H
