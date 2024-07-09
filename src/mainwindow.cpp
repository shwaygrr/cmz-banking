#include "mainwindow.h"
#include "bankwidget.h"
#include "qscrollarea.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <regex>
#include "user.h"
#include <qwidget.h>
#include <QWidget>
#include <QComboBox>
#include <random>


string username, password, email;
string currentwindow;

User currentUser;

int amountToTransfer;

QComboBox *from, *to;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), layout(new QVBoxLayout), centralWidget(new QWidget(this))
{
    ui->setupUi(this);
    setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);

    qDebug() << "Program starting...";

    users.push_back(User("", "", ""));      // TESTING PURPOSES ONLY
    users.push_back(User("a", "a", "a"));
    users.push_back(User("b", "b", "b"));

    loadUI("mainwindow.ui");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadUI(const QString &uiFile)
{
    QString ui = ":/forms/" + uiFile;
    username = "";
    password = "";
    email = "";
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

bool MainWindow::CheckForUser(string username)
{
    for(int i = 0; i < (int)users.size(); i++)
    {
        if(users.at(i).GetUsername() == username)
        {
            return true;
        }
    }
    return false;
}

void MainWindow::Login()
{
    // DELETE COMMENT UNLESS TESTING, this is so you can log in with nothing in username and password
    //if(username.empty() && password.empty()){ return; }

    qDebug() << "Attempting to login...";

    User tempUser;

    for(int i = 0; i < (int)users.size(); i++)
    {
        tempUser = users.at(i);
        qDebug() << tempUser.GetUsername() << " " << tempUser.GetPassword() << " " << tempUser.GetBankAccounts().size();
        if(users.at(i).GetUsername() == username)
        {
            if(users.at(i).GetPassword() == password)
            {
                tempUser.AddToActivityLog("Successfully logged in...Welcome back " + username + "!");
                qDebug() << "Successfully logged in...Welcome back" << username << "!";
                currentUser = tempUser;
                loadUI("dashboardwindow.ui");
                return;
            }
            else
            {
                users.at(i).AddToActivityLog("Someone has tried logging in with your username with an incorrect password");
                qDebug() << "Failed to login...Invalid password";
                return;
            }
        }
    }
    qDebug() << "Failed to login...Invalid credentials";
}

bool MainWindow::CheckPassword(string password)
{
    if(password.length() < 8){ return false; }
    else
    {
        for(int i = 0; i < (int)password.length(); i++)
        {
            if(!isdigit(password[i]) && !isalpha(password[i]))
            {
                break;
            }
            if(i == (int)password.length() - 1){return false;}
        }
        for(int i = 0; i < (int)password.length(); i++)
        {
            if(isdigit(password[i]))
            {
                break;
            }
            if(i == (int)password.length() - 1){return false;}
        }
        for(int i = 0; i < (int)password.length(); i++)
        {
            if(isalpha(password[i]))
            {
                if(isupper(password[i]))
                {
                    break;
                }
            }
            if(i == (int)password.length() - 1){return false;}
        }
    }
    return true;
}

int MainWindow::CheckEmail(const string &email)
{
    for(int i = 0; i < (int)users.size(); i++)
    {
        if(users.at(i).GetEmail() == email)
        {
            return 0;
        }
    }

    const regex pattern(R"((^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$))");
    if(!regex_match(email, pattern))
    {
        return 1;
    }
    else{
        return 2;
    }
}

void MainWindow::CreateUser()
{
    int valid = CheckEmail(email);

    if(valid == 0)
    {
        qDebug() << "Failed to create...Email is already in use...";
        return;
    }
    else if(valid == 1)
    {
        qDebug() << "Failed to create...Email is not a valid address.";
        return;
    }

    if(!CheckForUser(username))
    {
        if(username.empty())
        {
            qDebug() << "Failed to create...Username cannot be empty.";
            return;
        }
    }
    else
    {
        qDebug() << "Failed to create...username already in use";
        return;
    }

    if(!CheckPassword(password))
    {
        qDebug() << "Password does not meet the requirements.";
        return;
    }

    User newUser = User(username, password, email);
    users.push_back(newUser);
    newUser.AddToActivityLog("Successfully created user...Welcome" + username + "!");
    qDebug() << "Successfully created user...Welcome" << username << "!";

    loadUI("mainwindow.ui");
}

bool MainWindow::VerifyAction()
{
    username = currentUser.GetUsername();

    for(int i = 0; i < (int)users.size(); i++)
    {
        if(users[i].GetUsername() == username)
        {
            return users[i].GetPassword() == currentUser.GetPassword();
        }
    }
    return false;
}

BankWidget* MainWindow::LoadAccount(string accountNumber, string accountType, string accountBalance)
{
    BankWidget *widget = new BankWidget(this);

    widget->SetAccountNumber(accountNumber);
    widget->SetAccountType(accountType);
    widget->SetAccountBalance(accountBalance);

    return widget;
}


BankWidget* MainWindow::CreateAccount(string accountNumber, string accountType, string accountBalance)
{
    BankWidget *widget = new BankWidget(this);

    widget->SetAccountNumber(accountNumber);
    widget->SetAccountType(accountType);
    widget->SetAccountBalance(accountBalance);

    currentUser.AddToActivityLog("Added a new account | Account Number: " + accountNumber + ", Account Type: " + accountType + ", Account Balance: $" + accountBalance);
    currentUser.CreateBankAccount(stoi(accountNumber), accountType, stof(accountBalance));

    return widget;
}

void MainWindow::SaveUser()
{
    for(int i = 0; i < (int)users.size(); i++)
    {
        if(users.at(i).GetUsername() == currentUser.GetUsername())
        {
            users.at(i).SetActivityLog(currentUser.GetActivityLog());
            users.at(i).SetBankAccounts(currentUser.GetBankAccounts());
        }
    }
}

void MainWindow::setupButtonConnections()
{
    // Main Elements
    if(currentwindow == "mainwindow.ui")
    {
        QLineEdit *main_input_username = centralWidget->findChild<QLineEdit*>("main_input_username");
        if(main_input_username)
        {
            connect(main_input_username, &QLineEdit::textChanged, this, [](const QString &text)
                    {
                        username = text.toStdString();
                    });
        }
        QLineEdit *main_input_password = centralWidget->findChild<QLineEdit*>("main_input_password");
        if(main_input_password)
        {
            connect(main_input_password, &QLineEdit::textChanged, this, [](const QString &text)
                    {
                        password = text.toStdString();
                    });
        }
        QPushButton *main_button_login = centralWidget->findChild<QPushButton*>("main_button_login");
        if(main_button_login)
        {
            connect(main_button_login, &QPushButton::clicked, this, [this]() { Login(); });
        }
        QPushButton *main_button_create = centralWidget->findChild<QPushButton*>("main_button_createaccount");
        if(main_button_create)
        {
            connect(main_button_create, &QPushButton::clicked, this, [this]() { loadUI("createwindow.ui"); });
        }
    }

    // Create Elements
    else if(currentwindow == "createwindow.ui")
    {
        QLineEdit *create_input_username = centralWidget->findChild<QLineEdit*>("create_input_username");
        if(create_input_username)
        {
            connect(create_input_username, &QLineEdit::textChanged, this, [](const QString &text)
                    {
                        username = text.toStdString();
                    });
        }
        QLineEdit *create_input_password = centralWidget->findChild<QLineEdit*>("create_input_password");
        if(create_input_password)
        {
            connect(create_input_password, &QLineEdit::textChanged, this, [](const QString &text)
                    {
                        password = text.toStdString();
                    });
        }
        QLineEdit *create_input_email = centralWidget->findChild<QLineEdit*>("create_input_email");
        if(create_input_email)
        {
            connect(create_input_email, &QLineEdit::textChanged, this, [](const QString &text)
                    {
                        email = text.toStdString();
                    });
        }
        QPushButton *create_button_createaccount = centralWidget->findChild<QPushButton*>("create_button_createaccount");
        if(create_button_createaccount)
        {
            connect(create_button_createaccount, &QPushButton::clicked, this, [this]() { CreateUser(); });
        }
        QPushButton *create_button_backtologin = centralWidget->findChild<QPushButton*>("create_button_backtologin");
        if(create_button_backtologin)
        {
            connect(create_button_backtologin, &QPushButton::clicked, this, [this]() { loadUI("mainwindow.ui"); });
        }
    }

    // Dashboard Elements
    else if(currentwindow == "dashboardwindow.ui")
    {
        QScrollArea *scrollArea = centralWidget->findChild<QScrollArea*>("scrollArea");
        QWidget *scrollWidget = scrollArea->findChild<QWidget*>("contents");
        QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);
        scrollLayout->stretch(0);

        QPushButton *goto_transfer = centralWidget->findChild<QPushButton*>("button_transfer");
        if(goto_transfer)
        {
            connect(goto_transfer, &QPushButton::clicked, this, [this]() { loadUI("transferwindow.ui"); });
        }

        QPushButton *goto_profile = centralWidget->findChild<QPushButton*>("button_profile");
        if(goto_profile)
        {
            connect(goto_profile, &QPushButton::clicked, this, [this]() { loadUI("profilewindow.ui"); });
        }

        QPushButton *add_account = centralWidget->findChild<QPushButton*>("button_add_account");
        if(add_account)
        {
            connect(add_account, &QPushButton::clicked, this, [this]()
            {
                QScrollArea *scrollArea = centralWidget->findChild<QScrollArea*>("scrollArea");
                QWidget *scrollWidget = scrollArea->findChild<QWidget*>("contents");

                random_device rd;
                mt19937 gen(rd());

                int min = 1000000000;
                int max = 9999999999;

                uniform_int_distribution<> distr(min, max);

                int randomInt = distr(gen);

                scrollWidget->findChild<QVBoxLayout*>()->addWidget(CreateAccount(QString::number(randomInt).toStdString(), "Checking", "420.69"));
            });
        }

        //Load user bank accounts
        for(int i = 0; i < (int)currentUser.GetBankAccounts().size(); i++)
        {
            BankAccount userAccount = currentUser.GetBankAccounts().at(i);
            QString strNumber = QString::number(userAccount.getBalance(), 'f', 2);
            scrollWidget->findChild<QVBoxLayout*>()->addWidget(LoadAccount(to_string(userAccount.getNumber()), userAccount.getType(), strNumber.toStdString()));
        }
    }

    // Transfer Elements
    else if(currentwindow == "transferwindow.ui")
    {
        QList<QString> accounts;

        amountToTransfer = 0;

        for(int i = 0; i < (int)currentUser.GetBankAccounts().size(); i++)
        {
            BankAccount userAccount = currentUser.GetBankAccounts().at(i);
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
                amountToTransfer = stoi(text.toStdString());
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
            connect(transfer, &QPushButton::clicked, this, []()
            {
                QString from_text = from->currentText();
                QString to_text = to->currentText();

                BankAccount from_account = currentUser.FindBankAccount(from_text.left(11).toInt());
                BankAccount to_account = currentUser.FindBankAccount(to_text.left(11).toInt());

                qDebug() << "Transferred $" << amountToTransfer << " From: " << from_account.getNumber() << " To: " << to_account.getNumber();
            });
        }
    }

    else if(currentwindow == "profilewindow.ui")
    {
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
                SaveUser();
                loadUI("mainwindow.ui");
            });
        }
    }
}
