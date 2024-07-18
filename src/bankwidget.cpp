#include "BankWidget.h"

BankWidget::BankWidget(QWidget *parent)
    : QWidget(parent)
{ 
    numberLabel = new QLabel(this);
    typeLabel = new QLabel(this);
    balanceLabel = new QLabel(this);
    trashButton = new QPushButton(this);

    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->addWidget(numberLabel);
    layout->addWidget(typeLabel);
    layout->addWidget(balanceLabel);
    layout->addWidget(trashButton);

    layout->setSpacing(20);

    setLayout(layout);
}

void BankWidget::setAccountNumber(string &accountNumber)
{
    numberLabel->setText(QString::fromStdString(accountNumber));
}
void BankWidget::setAccountType(string &accountType)
{
    typeLabel->setText(QString::fromStdString(accountType));
}
void BankWidget::setAccountBalance(string &accountBalance)
{
    balanceLabel->setText("$" + QString::fromStdString(accountBalance));
}

string BankWidget::getAccountNumber()
{
    return numberLabel->text().toStdString();
}
string BankWidget::getAccountType()
{
    return typeLabel->text().toStdString();
}
string BankWidget::getAccountBalance()
{
    return balanceLabel->text().toStdString();
}

QPushButton* BankWidget::getTrashButton()
{
    return trashButton;
}
