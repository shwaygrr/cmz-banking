#include "mainwindow.h"
#include "BankWidget.h"
#include "ActivityWidget.h"
#include "qscrollarea.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <regex>
#include "user.h"
#include <qwidget.h>
#include <QWidget>
#include <QComboBox>
#include <random>

string currentwindow;

User *currentUser;

float amountToTransfer;

QComboBox *from, *to;
QButtonGroup *radioGroup;
int from_index, to_index;
QString full_name, username, password; //set to main user after

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), layout(new QVBoxLayout), centralWidget(new QWidget(this))
{
    ui->setupUi(this);
    setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);

    qDebug() << "Program starting...";

    loadUI("mainwindow.ui");
}


MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::loadUI(const QString &uiFile) {
    QString ui = ":/forms/" + uiFile;

    full_name = "";
    username = "";
    password = "";

    qDebug() << "Attempting to navigate to" << ui;

    QFile file(ui);
    file.open(QFile::ReadOnly);
    QWidget *widget = loader.load(&file, this);
    file.close();

    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    layout->addWidget(widget);

    currentwindow = uiFile.toStdString();

    setupButtonConnections();
}


void MainWindow::login(const QString& username_, const QString& password_) {
    DB db;

    if(username_.isEmpty() && password_.isEmpty()) {
        qDebug() << "Username and Password field can't be empty";
        return;
    };

    qDebug() << "Attempting to login...";

    if(db.authenticate(username_, password_))
        // qDebug() << "logged in";
        loadUI("dashboardwindow.ui"); //Mark: not sure how loading next window should work -- app crashes here
}


void MainWindow::createUser(QString& full_name_, QString& username_, QString& password_) {
    DB db; //need to make db global

    if (!checkPasswordFormat(password_.toStdString())) {
        qDebug() << "Password does not meet the requirements.";
        return;
    }

    db.createUser(full_name_, username_, password_);
    qDebug() << "Successfully created user...Welcome" << username_ << "!";
    loadUI("mainwindow.ui");
}


bool MainWindow::verifyAction()
{
    // username = currentUser->getUsername();

    // for(int i = 0; i < (int)users.size(); i++)
    // {
    //     if(users[i].getUsername() == username)
    //     {
    //         return users[i].getPassword() == currentUser->getPassword();
    //     }
    // }
    // return false;
}

void MainWindow::deleteAllAccountsUI()
{
    QScrollArea *scrollArea = centralWidget->findChild<QScrollArea*>("scrollArea");
    QWidget *scrollWidget = scrollArea->findChild<QWidget*>("contents");
    QVBoxLayout *qvLayout = scrollWidget->findChild<QVBoxLayout*>();

    QLayoutItem *item;
    while((item = qvLayout->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }
}

void MainWindow::loadAllAccounts()
{
    deleteAllAccountsUI();
    QScrollArea *scrollArea = centralWidget->findChild<QScrollArea*>("scrollArea");
    QWidget *scrollWidget = scrollArea->findChild<QWidget*>("contents");

    for(int i = 0; i < (int)currentUser->getBankAccounts().size(); i++)
    {
        BankAccount userAccount = currentUser->getBankAccounts().at(i);
        QString strNumber = QString::number(userAccount.getBalance(), 'f', 2);

        scrollWidget->findChild<QVBoxLayout*>()->addWidget(loadAccount(to_string(userAccount.getNumber()), userAccount.getType(), strNumber.toStdString()));
    }
}

BankWidget* MainWindow::loadAccount(string accountNumber, string accountType, string accountBalance)
{
    BankWidget *widget = new BankWidget(this);

    widget->setAccountNumber(accountNumber);
    widget->setAccountType(accountType);
    widget->setAccountBalance(accountBalance);

    connect(widget->getTrashButton(), &QPushButton::clicked, this, [this, widget]() { deleteAccount(widget->getAccountNumber()); });

    return widget;
}

BankWidget* MainWindow::createAccount(string accountNumber, string accountType, string accountBalance)
{
    BankWidget *widget = new BankWidget(this);

    widget->setAccountNumber(accountNumber);
    widget->setAccountType(accountType);
    widget->setAccountBalance(accountBalance);

    currentUser->addToActivityLog("Added a new account | Account Number: " + accountNumber + ", Account Type: " + accountType + ", Account Balance: $" + accountBalance);
    currentUser->createBankAccount(stoi(accountNumber), accountType, stof(accountBalance));

    return widget;
}

ActivityWidget* MainWindow::createActivity(string activity, string time)
{
    ActivityWidget *widget = new ActivityWidget(this);

    widget->setActivity(activity);
    widget->setTime(time);

    return widget;
}

void MainWindow::deleteAccount(string accountNumber) {
    currentUser->deleteBankAccount(stoi(accountNumber));

    loadAllAccounts();
}

void MainWindow::setupButtonConnections() {
    //Login
    if (currentwindow == "mainwindow.ui") {
        QLineEdit *main_input_username = centralWidget->findChild<QLineEdit*>("main_input_username");
        if(main_input_username)
            connect(main_input_username, &QLineEdit::textChanged, this, [](const QString &text) {
                username = text;
            });

        QLineEdit *main_input_password = centralWidget->findChild<QLineEdit*>("main_input_password");
        if(main_input_password)
            connect(main_input_password, &QLineEdit::textChanged, this, [](const QString &text) {
                password = text;
            });

        QPushButton *main_button_login = centralWidget->findChild<QPushButton*>("main_button_login");
        if(main_button_login)
            connect(main_button_login, &QPushButton::clicked, this, [this]() {
                login(username, password);

            });

        QPushButton *main_button_create = centralWidget->findChild<QPushButton*>("main_button_createaccount");
        if(main_button_create)
            connect(main_button_create, &QPushButton::clicked, this, [this]() {
                loadUI("createwindow.ui");
            });
    }

    //Create Account
    else if(currentwindow == "createwindow.ui") {
        QLineEdit *create_input_full_name = centralWidget->findChild<QLineEdit*>("create_input_full_name");
        if(create_input_full_name)
            connect(create_input_full_name, &QLineEdit::textChanged, this, [](const QString &text) {
                full_name = text;
            });

        QLineEdit *create_input_username = centralWidget->findChild<QLineEdit*>("create_input_username");
        if(create_input_username)
            connect(create_input_username, &QLineEdit::textChanged, this, [](const QString &text) {
                username = text;
            });

        QLineEdit *create_input_password = centralWidget->findChild<QLineEdit*>("create_input_password");
        if(create_input_password) {
            connect(create_input_password, &QLineEdit::textChanged, this, [](const QString &text) {
                password = text;
            });
        }

        //create account page
        QPushButton *create_button_createaccount = centralWidget->findChild<QPushButton*>("create_button_createaccount");
        if(create_button_createaccount)
            connect(create_button_createaccount, &QPushButton::clicked, this, [this]() {
                createUser(full_name, username, password);
            });


        QPushButton *create_button_backtologin = centralWidget->findChild<QPushButton*>("create_button_backtologin");
        if(create_button_backtologin)
            connect(create_button_backtologin, &QPushButton::clicked, this, [this]() {
                loadUI("mainwindow.ui");
            });
    }

    // Dashboard Elements
    else if(currentwindow == "dashboardwindow.ui") {
        QScrollArea *scrollArea = centralWidget->findChild<QScrollArea*>("scrollArea");
        QWidget *scrollWidget = scrollArea->findChild<QWidget*>("contents");
        QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);
        scrollLayout->stretch(0);

        QPushButton *goto_transfer = centralWidget->findChild<QPushButton*>("button_transfer");
        if(goto_transfer)
            connect(goto_transfer, &QPushButton::clicked, this, [this]() {
                loadUI("transferwindow.ui");
            });

        QPushButton *goto_profile = centralWidget->findChild<QPushButton*>("button_profile");
        if(goto_profile)
            connect(goto_profile, &QPushButton::clicked, this, [this]() {
                loadUI("profilewindow.ui");
            });

        QPushButton *add_account = centralWidget->findChild<QPushButton*>("button_add_account");
        if(add_account)
            connect(add_account, &QPushButton::clicked, this, [this]() {
                    QScrollArea *scrollArea = centralWidget->findChild<QScrollArea*>("scrollArea");
                    QWidget *scrollWidget = scrollArea->findChild<QWidget*>("contents");

                    random_device rd;
                    mt19937 gen(rd());

                    int min = 100000000;
                    int max = 999999999;

                    uniform_int_distribution<> distr(min, max);

                    int randomInt = distr(gen);

                    BankWidget *newAccount = createAccount(QString::number(randomInt).toStdString(), "Checking", "420.69");

                    connect(newAccount->getTrashButton(), &QPushButton::clicked, this, [this, newAccount]() { deleteAccount(newAccount->getAccountNumber()); });

                    scrollWidget->findChild<QVBoxLayout*>()->addWidget(newAccount);
                });
        //Load user bank accounts
        loadAllAccounts();
    }

    // Transfer Elements
    else if(currentwindow == "transferwindow.ui") {
        QList<QString> accounts;

        amountToTransfer = 0;

        for(int i = 0; i < (int)currentUser->getBankAccounts().size(); i++)
        {
            BankAccount userAccount = currentUser->getBankAccounts().at(i);
            QString strNumber = QString::number(userAccount.getBalance(), 'f', 2);
            accounts.push_back(QString::fromStdString(to_string(userAccount.getNumber()) + "     " + userAccount.getType() + "     $" + strNumber.toStdString()));
        }

        from = centralWidget->findChild<QComboBox*>("from");
        if(from)
        {
            from->addItems(accounts);
        }

        to = centralWidget->findChild<QComboBox*>("to");
        if(to)
        {
            to->addItems(accounts);
        }

        QLineEdit *amount = centralWidget->findChild<QLineEdit*>("amount");
        if(amount)
        {
            connect(amount, &QLineEdit::textChanged, this, [](const QString &text)
                    {
                        amountToTransfer = stof(text.toStdString());
                    });
        }
        QPushButton *goto_dashboard = centralWidget->findChild<QPushButton*>("button_back");
        if(goto_dashboard)
        {
            connect(goto_dashboard, &QPushButton::clicked, this, [this]() { loadUI("dashboardwindow.ui"); });
        }

        QPushButton *transfer = centralWidget->findChild<QPushButton*>("button_transfer");
        if(transfer)
        {
            connect(transfer, &QPushButton::clicked, this, [this]()
                    {
                        if(from->currentIndex() == to->currentIndex()) { return; }

                        from_index = from->currentIndex();
                        to_index = to->currentIndex();

                        QString from_text = from->currentText();
                        QString to_text = to->currentText();

                        BankAccount *from_account = currentUser->findBankAccount(from_text.left(11).toInt());
                        BankAccount *to_account = currentUser->findBankAccount(to_text.left(11).toInt());

                        QList<QString> accounts;

                        currentUser->transferMoney(amountToTransfer, *from_account, *to_account);

                        for(int i = 0; i < (int)currentUser->getBankAccounts().size(); i++)
                        {
                            BankAccount userAccount = currentUser->getBankAccounts().at(i);
                            QString strNumber = QString::number(userAccount.getBalance(), 'f', 2);
                            accounts.push_back(QString::fromStdString(to_string(userAccount.getNumber()) + "     " + userAccount.getType() + "     $" + strNumber.toStdString()));
                        }

                        from = centralWidget->findChild<QComboBox*>("from");
                        to = centralWidget->findChild<QComboBox*>("to");

                        from->clear();
                        to->clear();

                        from->addItems(accounts);
                        to->addItems(accounts);

                        from->setCurrentIndex(from_index);
                        to->setCurrentIndex(to_index);
                    });
        }
    }

    // Profile Elements
    else if(currentwindow == "profilewindow.ui") {
        QPushButton *goto_dashboard = centralWidget->findChild<QPushButton*>("button_back");
        if(goto_dashboard)
        {
            connect(goto_dashboard, &QPushButton::clicked, this, [this]() { loadUI("dashboardwindow.ui"); });
        }
        QPushButton *signout = centralWidget->findChild<QPushButton*>("button_signout");
        if(signout)
        {
            connect(signout, &QPushButton::clicked, this, [this]()
                    {
                        loadUI("mainwindow.ui");
                    });
        }
        QPushButton *goto_activitylog = centralWidget->findChild<QPushButton*>("button_activitylog");
        if(goto_activitylog)
        {
            connect(goto_activitylog, &QPushButton::clicked, this, [this]() { loadUI("activitylogwindow.ui"); });
        }
    }

    // Activity Elements
    else if(currentwindow == "activitylogwindow.ui") {
        QScrollArea *scrollArea = centralWidget->findChild<QScrollArea*>("scrollArea");
        QWidget *scrollWidget = scrollArea->findChild<QWidget*>("contents");
        QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);
        scrollLayout->stretch(0);

        for(int i = 0; i < (int)currentUser->getActivityLog().size(); i++) {
            ActivityWidget *newAccount = createActivity(currentUser->getActivityLog().at(i), "5:00 pm");
            scrollWidget->findChild<QVBoxLayout*>()->addWidget(newAccount);
        }

        QPushButton *goto_profile = centralWidget->findChild<QPushButton*>("button_profile");
        if(goto_profile)
            connect(goto_profile, &QPushButton::clicked, this, [this]() {
                loadUI("profilewindow.ui");
            });
    }
}
