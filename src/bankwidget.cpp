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

string BankWidget::GetAccountNumber()
{
    return numberLabel->text().toStdString();
}
string BankWidget::GetAccountType()
{
    return typeLabel->text().toStdString();
}
string BankWidget::GetAccountBalance()
{
    return balanceLabel->text().toStdString();
}

QPushButton* BankWidget::GetTrashButton()
{
    return trashButton;
}
