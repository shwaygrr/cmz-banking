#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <QDebug>
using namespace std;

//this is where you put helper function declarations

class User {
    private:
        QString full_name, username, created_at;
        int user_id;
    public:
        User();
        User(int user_id_, QString full_name_, QString username_, QString created_at);

        friend QDebug operator << (QDebug dbg, const User& user);

        int getUserId() const;
        QString getFullName() const;
        QString getUsername() const;
        QString getCreatedAt() const;
};

#endif // USER_H
