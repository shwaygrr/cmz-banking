#include "transaction.h"

Transaction::Transaction(User* send, BankAccount* sendAcc, User* receive, BankAccount* receiveAcc, float am) {
    sender = send;
    senderAccount = sendAcc;
    receiver = receive;
    receiverAccount = receiveAcc;
    amount = am;
}

Transaction::Transaction() {
}
