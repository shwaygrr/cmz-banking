#ifndef BANKWIDGET_H
#define BANKWIDGET_H

#include "qpushbutton.h"
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QString>


using namespace std;

class BankWidget : public QWidget
{
    Q_OBJECT
public:
    BankWidget(QWidget *parent = 0);

    void setAccountNumber(QString& accountNumber);
    void setAccountType(QString& accountType);
    void setAccountBalance(QString& accountBalance);

    QString getAccountNumber();
    QString getAccountType();
    QString getAccountBalance();

    QPushButton* getTrashButton();

    bool checkState();

private:
    QLabel *numberLabel;
    QLabel *typeLabel;
    QLabel *balanceLabel;
    QPushButton *trashButton;
};

#endif // BANKWIDGET_H
