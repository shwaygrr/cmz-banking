#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <iostream>
#include <string>

using namespace std;

class BankAccount
{
private:
    int number;
    float balance;
    string type;

public:
    BankAccount(int n, string t, float b);

    void setNumber(int n);
    void setType(string t);
    void setBalance(float b);

    int getNumber();
    string getType();
    float getBalance();

    void send(float amount, BankAccount receiver);
    void receive(float amount);
};

#endif // BANKACCOUNT_H
