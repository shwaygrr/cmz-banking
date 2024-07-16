#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <iostream>
#include <string>
#include "user.h"
#include "bankaccount.h"

using namespace std;

//Transaction will be user for transfering and sending money
class Transaction
{
private:
    User* sender;
    BankAccount* senderAccount;
    User* receiver;
    BankAccount* receiverAccount;
    float amount;

public:
    Transaction();
    Transaction(User* send, BankAccount* sendAcc, User* receive, BankAccount* receiveAcc, float am);

    User getSender();
    BankAccount getSenderAccount();
    User getReceiver();
    BankAccount getReceiverAccount();
    float getAmount();
};

#endif // TRANSACTION_H
