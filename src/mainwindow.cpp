#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <regex>
#include <user.h>

string username, password, email;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), layout(new QVBoxLayout), centralWidget(new QWidget(this))
{
    ui->setupUi(this);
    setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);

    qDebug() << "Program starting...";
    loadUI(":/forms/mainwindow.ui");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadUI(const QString &uiFile)
{
    username = "";
    password = "";
    QFile file(uiFile);
    file.open(QFile::ReadOnly);
    QWidget *widget = loader.load(&file, this);
    file.close();

    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    layout->addWidget(widget);

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
    if(username.empty() && password.empty()){ return; }

    qDebug() << "Attempting to login...";

    for(int i = 0; i < (int)users.size(); i++)
    {
        if(users.at(i).GetUsername() == username && users.at(i).GetPassword() == password)
        {
            qDebug() << "Successfully logged in...Welcome back" << username << "!";
            return;
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
    qDebug() << "Successfully created user...Welcome" << username << "!";

    loadUI(":/forms/mainwindow.ui");
}

void MainWindow::setupButtonConnections()
{
    // Main Elements
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
        connect(main_button_create, &QPushButton::clicked, this, [this]() { loadUI(":/forms/createwindow.ui"); });
    }


    // Create Elements
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
        connect(create_button_backtologin, &QPushButton::clicked, this, [this]() { loadUI(":/forms/mainwindow.ui"); });
    }
}
