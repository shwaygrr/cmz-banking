#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QWidget>
#include <QUiLoader>
#include <QVBoxLayout>
#include <QFile>
#include <QPushButton>
#include <QString>

#include "user.h"
#include "api/db.h"
#include "system.h"
#include "widgets/BankWidget.h"
#include "widgets/ActivityWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
using namespace std;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


private:
    QString current_window;
    System* system;

    Ui::MainWindow *ui;
    QVBoxLayout *layout;
    QWidget *centralWidget;
    QUiLoader loader;

    void loadUI(const QString &uiFile);
    void setupButtonConnections();

    void deleteAllAccountsUI();
    void loadAllAccounts();

    BankWidget* loadAccount(QString account_number, QString account_type, QString account_balance);
    ActivityWidget* createActivityWidget(const Activity& activity);
};
#endif // MAINWINDOW_H
