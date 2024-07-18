#include "transaction.h"

//used for transactions between users
Transaction::Transaction(string sender_name, int sender_account, string receiver_name, int receiver_account, float amount_transferred, time_t time) {
    sender = sender_name;
    sender_acc = sender_account;
    receiver = receiver_name;
    receiver_acc = receiver_account;
    amount = amount_transferred;
    created_at = ctime(&time);
}

//used for transfers between accounts of the same user
Transaction::Transaction(int sender_account, int receiver_account, float amount_transferred, time_t time) {
    sender_acc = sender_account;
    receiver_acc = receiver_account;
    amount = amount_transferred;
    created_at = ctime(&time);
}

//default constructor
Transaction::Transaction() {
}

string Transaction::transferLogEntry() {
    return("Transferred " + to_string(amount) + " from account " + to_string(sender_acc) + " to account " + to_string(receiver_acc) + "at" + created_at);
}

string Transaction::senderLogEntry() {
    return("Sent " + to_string(amount) + " from account " + to_string(sender_acc) + " to user " + receiver + " at " + created_at);
}

string Transaction::receiverLogEntry() {
    return("Received" + to_string(amount) + "from user " + sender + " at " + created_at);
}
