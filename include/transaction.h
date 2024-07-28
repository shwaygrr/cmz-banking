#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <iostream>
#include <string>
#include <ctime>
#include <QString>

using namespace std;

class Transaction {
private:
    QString transaction_type, created_at, description;
    int transaction_id, sender_id, receiver_id;
    float amount;

    bool updateBankAccountBalanceById(const int account_id, const float new_balance);
public:
    Transaction();
    Transaction(const QString& description, int sender_id_, int reciever_id_, float amount_);
    Transaction(int transaction_id_, const QString& transaction_type_, const QString& description_, int sender_id_, int receiver_id_, float amount_, const QString& created_at_);

    QString getTransactionType() const;
    float getAmount() const;
    QString getDescription() const;
    QString getCreatedAt() const;
    int getSenderId() const;
    int getReceiverId() const;
    int getTransactionId() const;

    void setTransactionType(QString transaction_type_);

    QString getLogEntry() const;

    friend QDebug operator << (QDebug dbg, const Transaction& transaction);
};

#endif // TRANSACTION_H
