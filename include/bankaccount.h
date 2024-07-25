#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <QString>
#include <QDebug>
#include <random>

class BankAccount {
private:
    int account_id, user_id;
    float balance;
    QString account_number, account_type, created_at;

    QString generateAccountNumber();

public:
    BankAccount();
    BankAccount(int user_id, QString account_type_, float balance_);
    BankAccount(int account_id_, int user_id_, QString account_number_, QString account_type_, float balance_, QString created_at_);
    void setAccountType(QString account_type);
    void setBalance(float balance);

    QString getAccountNumber() const;
    int getAccountId() const;
    int getAccountUserId() const;
    QString getAccountType() const;
    float getBalance() const;
    QString getCreatedAt() const;

    void send(float amount, BankAccount &receiver);
    void receive(float amount);

    friend QDebug operator << (QDebug dbg, const BankAccount& account);

};

#endif // BANKACCOUNT_H
