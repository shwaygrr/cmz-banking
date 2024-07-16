#include "user.h"

User::User() {
    user_id = 0;
    full_name = "";
    username = "";
    created_at = "";
}

User::User(int user_id_, QString full_name_, QString username_, QString created_at_) {
    user_id = user_id_;
    full_name = full_name_;
    username = username_;
    created_at = created_at;
}

QDebug operator << (QDebug dbg, const User& user) {
    QDebugStateSaver saver(dbg);
    dbg.nospace();

    if (user.getUserId() == 0) {
        dbg << "Invalid User -- User undefined";
    } else {
        dbg << "User ID: " << user.getUserId() << "\n"
            << "Full Name: " << user.getFullName() << "\n"
            << "Username: " << user.getUsername() << "\n"
            << "Created At: " << user.getCreatedAt();
    }
    return dbg;
}


//add security levels and error handling to getters later
int User::getUserId() const {
    return user_id;
}

QString User::getFullName() const {
    return full_name;
}

QString User::getUsername() const {
    return username;
}

QString User::getCreatedAt() const {
    return created_at;
}
