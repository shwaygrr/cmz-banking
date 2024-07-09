#include "BankWidget.h"

BankWidget::BankWidget(QWidget *parent)
    : QWidget(parent)
{

    numberLabel = new QLabel(this);
    typeLabel = new QLabel(this);
    balanceLabel = new QLabel(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(numberLabel);
    layout->addWidget(typeLabel);
    layout->addWidget(balanceLabel);

    layout->setSpacing(45);

    setLayout(layout);
}

void BankWidget::SetAccountNumber(string &accountNumber)
{
    numberLabel->setText(QString::fromStdString(accountNumber));
}
void BankWidget::SetAccountType(string &accountType)
{
    typeLabel->setText(QString::fromStdString(accountType));
}
void BankWidget::SetAccountBalance(string &accountBalance)
{
    balanceLabel->setText("$" + QString::fromStdString(accountBalance));
}
