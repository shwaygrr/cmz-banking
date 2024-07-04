#include "mainwindow.h"
#include "user.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <string.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString username, password;
vector<User> users;

bool CheckForUser(string username)
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

void Login(string username, string password)
{
    if(username.empty() && password.empty()){ return; }

    qDebug() << "Attempting to login...";

    for(int i = 0; i < (int)users.size(); i++)
    {
        if(users.at(i).GetUsername() == username && users.at(i).GetPassword() == password)
        {
            qDebug() << "Successfully logged in...Welcome back " << username;
            return;
        }
    }

    qDebug() << "Failed to login...Invalid credentials";
}

bool CheckPassword(string password)
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

void CreateUser(string username, string password)
{
    if(username.empty() && password.empty()){ return; }

    if(!CheckPassword(password)){ qDebug() << "Password does not meet the requirements."; return; }

    if(!CheckForUser(username))
    {
        User newUser = User(username, password);
        users.push_back(newUser);
        qDebug() << "Successfully created user...Welcome " << username << "!";
    }
    else
    {
        qDebug() << "Failed to create...username already in use";
    }
}

void MainWindow::on_button_login_clicked()
{
    username = ui->input_username->text();
    password = ui->input_password->text();

    Login(username.toStdString(), password.toStdString());
}



void MainWindow::on_button_createaccount_clicked()
{
    username = ui->input_username->text();
    password = ui->input_password->text();

    CreateUser(username.toStdString(), password.toStdString());
}

