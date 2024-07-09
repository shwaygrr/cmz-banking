#ifndef BANKWIDGET_H
#define BANKWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

using namespace std;

class BankWidget : public QWidget
{
    Q_OBJECT
public:
    BankWidget(QWidget *parent = 0);

    void SetAccountNumber(string &accountNumber);
    void SetAccountType(string &accountType);
    void SetAccountBalance(string &accountBalance);

private:
    QLabel *numberLabel;
    QLabel *typeLabel;
    QLabel *balanceLabel;
};

#endif // BANKWIDGET_H
