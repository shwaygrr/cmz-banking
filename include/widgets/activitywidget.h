#ifndef ACTIVITYWIDGET_H
#define ACTIVITYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QCheckBox>

#include "activity.h"

using namespace std;

class ActivityWidget : public QWidget {
Q_OBJECT
private:
    QLabel *activity_label;
    QLabel *time_label;



public:
    ActivityWidget(QWidget *parent = 0);
    void setDescription(const QString& description);
    void setTime(const QString& time);
    void setTextColor(const QColor& color);
};

#endif // ACTIVITYWIDGET_H
