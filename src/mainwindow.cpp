#include "mainwindow.h"
#include "widgets/BankWidget.h"
#include "widgets/ActivityWidget.h"
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

QString full_name, username, password; //set to main user after

// For the create account window
QString input_balance_text;

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

    if(db.authenticate(username_, password_)) {
        currentUser = db.getUserByUsername(username_);
        fetchUserData(currentUser->getUserId());
        loadUI("dashboardwindow.ui");
    }
}


void MainWindow::fetchUserData(const int id) {
    DB db;

    currentUser->setBankAccounts(db.getAllBankAccountsByUserId(id));
    currentUser->setActivityLog(db.getActivitiesByUserId(id));
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

void MainWindow::deleteAllAccountsUI() {
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

void MainWindow::loadAllAccounts() {
    deleteAllAccountsUI();
    QScrollArea *scrollArea = centralWidget->findChild<QScrollArea*>("scrollArea");
    QWidget *scrollWidget = scrollArea->findChild<QWidget*>("contents");

    fetchUserData(currentUser->getUserId());

    for(const BankAccount& account : currentUser->getBankAccounts()) {
        BankAccount user_account = account;
        QString strNumber = QString::number(user_account.getBalance(), 'f', 2);

        scrollWidget->findChild<QVBoxLayout*>()->addWidget(loadAccount(user_account.getAccountNumber(), user_account.getAccountType(), strNumber));
    }
}

BankWidget* MainWindow::loadAccount(QString accountNumber, QString accountType, QString accountBalance) {
    BankWidget *widget = new BankWidget(this);

    widget->setAccountNumber(accountNumber);
    widget->setAccountType(accountType);
    widget->setAccountBalance(accountBalance);

    connect(widget->getTrashButton(), &QPushButton::clicked, this, [this, widget]() {
        deleteAccount(widget->getAccountNumber());
    });

    return widget;
}

void MainWindow::createAccount(const QString& account_type, const float account_balance) {
    currentUser->createBankAccount(account_type, account_balance);
}

ActivityWidget* MainWindow::createActivityWidget(const Activity& activity) {
    ActivityWidget *widget = new ActivityWidget(this);

    widget->setDescription(activity.getDescription());
    widget->setTime(activity.getCreatedAt());

    return widget;
}

void MainWindow::deleteAccount(const QString& account_number) {
    if(currentUser->deleteBankAccount(account_number))
        loadAllAccounts();
}

void MainWindow::setupButtonConnections() {
    //Login
    static QString full_name, username, password;
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

                loadUI("createaccountwindow.ui");
                });
        //Load user bank accounts
        loadAllAccounts();
    }

    // Transfer Elements
    else if(currentwindow == "transferwindow.ui") {
        //need to move to and from variables --- Probably needs to be static
        QList<QString> accounts;
        static int amountToTransfer = 0;
        static QString description = "";
        static QComboBox *from, *to;
        static int from_index, to_index;

        for(const BankAccount& account : currentUser->getBankAccounts()) {
            QString strNumber = QString::number(account.getBalance(), 'f', 2);
            accounts.push_back(account.getAccountNumber() + "     " + account.getAccountType() + "     $" + strNumber);
        }

        from = centralWidget->findChild<QComboBox*>("from");
        if(from) from->addItems(accounts);

        to = centralWidget->findChild<QComboBox*>("to");
        if(to) to->addItems(accounts);


        QLineEdit *amount = centralWidget->findChild<QLineEdit*>("amount");
        if(amount)
            connect(amount, &QLineEdit::textChanged, this, [](const QString &text) {
                amountToTransfer = stof(text.toStdString());
            });

        QLineEdit *description_input = centralWidget->findChild<QLineEdit*>("description");
        if(description_input)
            connect(description_input, &QLineEdit::textChanged, this, [](const QString &text) {
                description = text;
            });

        //buttons
        QPushButton *goto_dashboard = centralWidget->findChild<QPushButton*>("button_back");
        if(goto_dashboard)
            connect(goto_dashboard, &QPushButton::clicked, this, [this]() { loadUI("dashboardwindow.ui"); });


        QPushButton *transfer = centralWidget->findChild<QPushButton*>("button_transfer");
        if(transfer)
            connect(transfer, &QPushButton::clicked, this, [this]() {
                DB db;
                if(from->currentIndex() == to->currentIndex()) {
                    //show error
                    return;
                }

                from_index = from->currentIndex();
                to_index = to->currentIndex();

                QString from_text = from->currentText();
                QString to_text = to->currentText();

                BankAccount *from_account = currentUser->findBankAccount(from_text.left(10));
                BankAccount *to_account = currentUser->findBankAccount(to_text.left(10));

                Transaction transaction("description", from_account->getAccountId(), to_account->getAccountId(), amountToTransfer);

                if(db.createTransaction(transaction)) {
                    db.createActivity(Activity(currentUser->getUserId(), transaction));
                };

                loadUI("dashboardwindow.ui");
            });
    }

    // Profile Elements
    else if(currentwindow == "profilewindow.ui") {
        static QString new_full_name, new_username, new_password;

        QPushButton *goto_dashboard = centralWidget->findChild<QPushButton*>("button_back");
        if(goto_dashboard)
            connect(goto_dashboard, &QPushButton::clicked, this, [this]() { loadUI("dashboardwindow.ui"); });

        QPushButton *signout = centralWidget->findChild<QPushButton*>("button_signout");
        if(signout)
            connect(signout, &QPushButton::clicked, this, [this]() {
                DB db;
                db.createActivity(Activity(currentUser->getUserId(), "Signed out"));
                loadUI("mainwindow.ui");
            });

        QPushButton *delete_account = centralWidget->findChild<QPushButton*>("profile_button_deleteaccount");
        if(delete_account)
            connect(delete_account, &QPushButton::clicked, this, [this]() {
                DB db;
                db.deleteUserById(currentUser->getUserId());
                loadUI("mainwindow.ui");
                delete currentUser;
            });

        QPushButton *goto_activitylog = centralWidget->findChild<QPushButton*>("button_activitylog");
        if(goto_activitylog)
            connect(goto_activitylog, &QPushButton::clicked, this, [this]() { loadUI("activitylogwindow.ui"); });

        //name field and button
        QLineEdit *profile_input_fullname = centralWidget->findChild<QLineEdit*>("profile_input_fullname");
        if(profile_input_fullname)
            connect(profile_input_fullname, &QLineEdit::textChanged, this, [](const QString &text) { new_full_name = text; });

        QPushButton *change_full_name_button = centralWidget->findChild<QPushButton*>("profile_button_changefullname");
        if(change_full_name_button)
            connect(change_full_name_button, &QPushButton::clicked, this, [this]() {
                DB db;
                db.updateUserById(currentUser->getUserId(), "full_name", new_full_name);
            });

        //username field and button
        QLineEdit *profile_input_username = centralWidget->findChild<QLineEdit*>("profile_input_username");
        if(profile_input_username)
            connect(profile_input_username, &QLineEdit::textChanged, this, [](const QString &text) { new_username = text; });

        QPushButton *change_username_button = centralWidget->findChild<QPushButton*>("profile_button_changeusername");
        if(change_username_button)
            connect(change_username_button, &QPushButton::clicked, this, [this]() {
                DB db;
                db.updateUserById(currentUser->getUserId(), "username", new_username);
            });

        //password field and button
        QLineEdit *profile_input_password= centralWidget->findChild<QLineEdit*>("profile_input_password");
        if(profile_input_password)
            connect(profile_input_password, &QLineEdit::textChanged, this, [](const QString &text) { new_password = text; });

        QPushButton *change_password_button = centralWidget->findChild<QPushButton*>("profile_button_changepassword");
        if(change_password_button)
            connect(change_password_button, &QPushButton::clicked, this, [this]() {
                DB db;
                Hash hash;
                db.updateUserById(currentUser->getUserId(), "password_hash", QString::fromStdString(hash.hash(new_password.toStdString())));
            });
    }

    // Activity Elements
    else if(currentwindow == "activitylogwindow.ui") {
        fetchUserData(currentUser->getUserId());
        QScrollArea *scrollArea = centralWidget->findChild<QScrollArea*>("scrollArea");
        QWidget *scrollWidget = scrollArea->findChild<QWidget*>("contents");
        QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);
        scrollLayout->stretch(0);

        for (const Activity& activity : currentUser->getActivityLog()) {
            ActivityWidget *activity_widget = createActivityWidget(activity);
            scrollWidget->findChild<QVBoxLayout*>()->addWidget(activity_widget);
        }

        QPushButton *goto_profile = centralWidget->findChild<QPushButton*>("button_profile");
        if(goto_profile)
            connect(goto_profile, &QPushButton::clicked, this, [this]() {
                loadUI("profilewindow.ui");
            });
    }

    // Dashboard Elements
    else if(currentwindow == "createaccountwindow.ui") {

        QComboBox *combo_box = centralWidget->findChild<QComboBox*>("comboBox_type");

        QLineEdit *input_balance = centralWidget->findChild<QLineEdit*>("input_balance");
        if(input_balance)
            connect(input_balance, &QLineEdit::textChanged, this, [](const QString &text) { input_balance_text = text; });

        QPushButton *confirm = centralWidget->findChild<QPushButton*>("button_confirm");
        if(confirm)
            connect(confirm, &QPushButton::clicked, this, [this, combo_box, input_balance]() {
                //Create the account
                if(input_balance->text().toFloat() < 0){return;}
                createAccount(combo_box->currentText(), input_balance->text().toFloat());

                loadUI("dashboardwindow.ui");
            });
        QPushButton *cancel = centralWidget->findChild<QPushButton*>("button_cancel");
        if(cancel)
            connect(cancel, &QPushButton::clicked, this, [this]() {
                loadUI("dashboardwindow.ui");
            });
    }
}
