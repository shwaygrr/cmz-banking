#include "ActivityWidget.h"

ActivityWidget::ActivityWidget(QWidget *parent)
    : QWidget(parent)
{
    activityLabel = new QLabel(this);
    timeLabel = new QLabel(this);

    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->addWidget(activityLabel);
    layout->addWidget(timeLabel);

    setLayout(layout);
}

void ActivityWidget::SetActivity(string &activity)
{
    activityLabel->setText(QString::fromStdString(activity));
}
void ActivityWidget::SetTime(string &time)
{
    timeLabel->setText(QString::fromStdString(time));
}
