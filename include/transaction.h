#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <iostream>
#include <string>
#include <ctime>

using namespace std;

//Transaction will be user for transferring and sending money
class Transaction
{
private:
    string sender;
    int sender_acc;
    string receiver;
    int receiver_acc;
    float amount;
    string created_at;

public:
    Transaction();
    Transaction(int sender_account, int receiver_account, float amount_transferred, time_t time);
    Transaction(string sender_name, int sender_account, string receiver_name, int receiver_account, float amount_transferred, time_t time);

    string transferLogEntry();
    string senderLogEntry();
    string receiverLogEntry();
};

#endif // TRANSACTION_H
