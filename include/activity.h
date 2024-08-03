#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <QString>
#include <QDebug>
#include "transaction.h"

class Activity {
private:
    int activity_id, user_id;
    bool validated;
    QString description, created_at, signature;

public:
    Activity();
    Activity(const int user_id_, const QString& description_);
    Activity(const int user_id_, const Transaction& transaction);
    Activity(const int activity_id_, const int user_id_, const QString& description_, const QString& signature_, const QString& created_at_);

    int getActivityId() const;
    int getUserId() const;
    QString getDescription() const;
    QString getCreatedAt() const;
    QString getSignature() const;
    bool getValidated() const;

    void setDescription(const QString& description_);
    void setSignature(const QString& signature_);
    void setValidated(const bool is_validated);
    friend QDebug operator<<(QDebug dbg, const Activity& activity);
};

#endif // ACTIVITY_H
