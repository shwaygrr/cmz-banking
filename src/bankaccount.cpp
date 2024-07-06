#include "bankaccount.h"

BankAccount::BankAccount(int n, string t, float b) {
    number = n;
    type = t;
    balance = b;
}

void BankAccount::setNumber(int n) {
    number = n;
}

void BankAccount::setType(string t) {
    type = t;
}

void BankAccount::setBalance(float b) {
    balance = b;
}

int BankAccount::getNumber() {
    return number;
}

string BankAccount::getType() {
    return type;
}

float BankAccount::getBalance() {
    return balance;
}

void BankAccount::send(float amount, BankAccount receiver) {
    balance -= amount;
    receiver.receive(amount);
}

void BankAccount::receive(float amount) {
    balance += amount;
}
