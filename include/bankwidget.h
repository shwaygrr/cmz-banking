#ifndef BANKWIDGET_H
#define BANKWIDGET_H

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

    void SetAccountNumber(string &accountNumber);
    void SetAccountType(string &accountType);
    void SetAccountBalance(string &accountBalance);
    string GetAccountNumber();
    string GetAccountType();
    string GetAccountBalance();
    QRadioButton* GetRadioButton();

    bool CheckState();

private:
    QLabel *numberLabel;
    QLabel *typeLabel;
    QLabel *balanceLabel;
    QRadioButton *radio;
};

#endif // BANKWIDGET_H
