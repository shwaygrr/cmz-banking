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

    void setAccountNumber(const QString& accountNumber);
    void setAccountType(const QString& accountType);
    void setAccountBalance(const QString& accountBalance);

    QString getAccountNumber() const;
    QString getAccountType() const;
    QString getAccountBalance() const;

    QPushButton* getTrashButton() const;

    bool checkState();

private:
    QLabel *numberLabel;
    QLabel *typeLabel;
    QLabel *balanceLabel;
    QPushButton *trashButton;
};

#endif // BANKWIDGET_H
