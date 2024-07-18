#ifndef BANKWIDGET_H
#define BANKWIDGET_H

#include "qpushbutton.h"
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QRadioButton>


using namespace std;

class BankWidget : public QWidget
{
    Q_OBJECT
public:
    BankWidget(QWidget *parent = 0);

    void setAccountNumber(string &accountNumber);
    void setAccountType(string &accountType);
    void setAccountBalance(string &accountBalance);

    string getAccountNumber();
    string getAccountType();
    string getAccountBalance();

    QPushButton* getTrashButton();

    bool checkState();

private:
    QLabel *numberLabel;
    QLabel *typeLabel;
    QLabel *balanceLabel;
    QPushButton *trashButton;
};

#endif // BANKWIDGET_H
