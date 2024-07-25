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

    void createAccount(const QString& accoun_type, const float account_balance);
    BankWidget* loadAccount(QString account_number, QString account_type, QString account_balance);
    ActivityWidget* createActivity(string activity, string time);
    void deleteAccount(const QString& account_number);

    bool verifyAction();

    void login(const QString& username_, const QString& password_);
    void createUser(QString& full_name_, QString& username_, QString& password_);

    User fetchUserData(const int id);
};
#endif // MAINWINDOW_H
