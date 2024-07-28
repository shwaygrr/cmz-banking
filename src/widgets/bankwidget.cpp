#include "widgets/BankWidget.h"

BankWidget::BankWidget(QWidget *parent) : QWidget(parent) {
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

void BankWidget::setAccountNumber(const QString& accountNumber) {
    numberLabel->setText(accountNumber);
}

void BankWidget::setAccountType(const QString& accountType) {
    typeLabel->setText(accountType);
}

void BankWidget::setAccountBalance(const QString& accountBalance) {
    balanceLabel->setText("$" + accountBalance);
}

QString BankWidget::getAccountNumber() const {
    return numberLabel->text();
}

QString BankWidget::getAccountType() const {
    return typeLabel->text();
}

QString BankWidget::getAccountBalance() const {
    return balanceLabel->text();
}

QPushButton* BankWidget::getTrashButton() const {
    return trashButton;
}
