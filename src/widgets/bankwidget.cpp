#include "widgets/BankWidget.h"

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

void BankWidget::setAccountNumber(QString &accountNumber) {
    numberLabel->setText(accountNumber);
}

void BankWidget::setAccountType(QString &accountType) {
    typeLabel->setText(accountType);
}

void BankWidget::setAccountBalance(QString &accountBalance) {
    balanceLabel->setText("$" + accountBalance);
}

QString BankWidget::getAccountNumber() {
    return numberLabel->text();
}

QString BankWidget::getAccountType() {
    return typeLabel->text();
}

QString BankWidget::getAccountBalance() {
    return balanceLabel->text();
}



QPushButton* BankWidget::getTrashButton() {
    return trashButton;
}
