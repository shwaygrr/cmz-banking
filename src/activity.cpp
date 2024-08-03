#include "activity.h"

// Default constructor
Activity::Activity()
    : activity_id(0), user_id(0), description(""), created_at(""), signature(""), validated(false) {}

Activity::Activity(const int user_id_, const QString& description_)
    : activity_id(0), user_id(user_id_), description(description_), created_at("") {}

Activity::Activity(int user_id_, const Transaction& transaction)
    : activity_id(0), user_id(user_id_), created_at("") {
    description = transaction.getLogEntry();
}

Activity::Activity(const int activity_id_, const int user_id_, const QString& description_, const QString& signature_, const QString& created_at_)
    : activity_id(activity_id_), user_id(user_id_), description(description_), signature(signature_), created_at(created_at_) {}

int Activity::getActivityId() const { return activity_id; }

int Activity::getUserId() const { return user_id; }

QString Activity::getDescription() const { return description; }

QString Activity::getCreatedAt() const { return created_at; }

QString Activity::getSignature() const { return signature; }

void Activity::setDescription(const QString& description_) { description = description_; }

bool Activity::getValidated() const { return validated; }

void Activity::setValidated(const bool is_validated) { validated = is_validated; }

void Activity::setSignature(const QString& signature_) {
    qDebug() << "signature: " << signature_;
    signature = signature_;
}

QDebug operator << (QDebug dbg, const Activity& activity) {
    QDebugStateSaver saver(dbg);
    dbg.nospace();

    dbg << "Activity ID: " << activity.getActivityId() << "\n"
        << "User ID: " << activity.getUserId() << "\n"
        << "Description: " << activity.getDescription() << "\n"
        << "Created at: " << activity.getCreatedAt() << "\n"
        << "Signature: " << activity.getSignature();
    return dbg;
}
