#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <QString>
#include <QDebug>
#include "transaction.h"

class Activity {
private:
    int activity_id, user_id;
    QString description, created_at;

public:
    Activity();
    Activity(const int user_id_, const QString& description_);
    Activity(const int user_id_, const Transaction& transaction);
    Activity(const int activity_id_, const int user_id_, const QString& description_, const QString& created_at_);

    int getActivityId() const;
    int getUserId() const;
    QString getDescription() const;
    QString getCreatedAt() const;
    friend QDebug operator<<(QDebug dbg, const Activity& activity);
};

#endif // ACTIVITY_H
