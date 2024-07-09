#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

    void Login();
    void CreateUser();

    bool CheckForUser(string username);
    bool CheckPassword(string password);

    bool VerifyAction();

    int CheckEmail(const string &email);
};
#endif // MAINWINDOW_H
