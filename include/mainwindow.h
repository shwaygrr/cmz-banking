#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "widgets/BankWidget.h"
#include "widgets/ActivityWidget.h"
#include <QMainWindow>
#include <QWidget>
#include <QUiLoader>
#include <QVBoxLayout>
#include <QFile>
#include <QPushButton>

#include "user.h"
#include "api/db.h"

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

    void saveUser();
    void deleteAllAccountsUI();
    void loadAllAccounts();

    BankWidget* createAccount(string accountNumber, string accountType, string accountBalance);
    BankWidget* loadAccount(string accountNumber, string accountType, string accountBalance);
    ActivityWidget* createActivity(string activity, string time);
    void deleteAccount(string accountNumber);

    bool verifyAction();

    void login(const QString& username_, const QString& password_);
    void createUser(QString& full_name_, QString& username_, QString& password_);
};
#endif // MAINWINDOW_H
