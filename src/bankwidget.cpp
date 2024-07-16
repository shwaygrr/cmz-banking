#include "BankWidget.h"

BankWidget::BankWidget(QWidget *parent)
    : QWidget(parent)
{ 
    numberLabel = new QLabel(this);
    typeLabel = new QLabel(this);
    balanceLabel = new QLabel(this);

    radio = new QRadioButton(this);

    radio->autoExclusive();

    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->addWidget(numberLabel);
    layout->addWidget(typeLabel);
    layout->addWidget(balanceLabel);
    layout->addWidget(radio);

    radio->setText("");

    layout->setSpacing(20);

    setLayout(layout);
}

bool BankWidget::CheckState()
{
    return radio->isChecked();
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
QRadioButton* BankWidget::GetRadioButton()
{
    return radio;
}
