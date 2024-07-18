#ifndef ACTIVITYWIDGET_H
#define ACTIVITYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QCheckBox>

using namespace std;

class ActivityWidget : public QWidget
{
    Q_OBJECT
private:
    QLabel *activityLabel;
    QLabel *timeLabel;



public:
    ActivityWidget(QWidget *parent = 0);
    void setActivity(string &activity);
    void setTime(string &time);


};

#endif // ACTIVITYWIDGET_H
