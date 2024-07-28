#include "widgets/activitywidget.h"

ActivityWidget::ActivityWidget(QWidget *parent) : QWidget(parent) {
    activity_label = new QLabel(this);
    time_label = new QLabel(this);

    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->addWidget(activity_label);
    layout->addWidget(time_label);

    setLayout(layout);
}

void ActivityWidget::setDescription(const QString& description) {
    activity_label->setText(description);
}

void ActivityWidget::setTime(const QString& time) {
    time_label->setText(time);
}
