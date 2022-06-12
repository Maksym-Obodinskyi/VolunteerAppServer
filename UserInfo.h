#ifndef USERINFO_H
#define USERINFO_H
#include <QString>

struct UserInfo
{
//    UserInfo(std::string _name
//       , std::string _lastName
//       , std::string _number
//       , std::string _photo
//       , double      _rating
//       , std::string _email
//       , std::string _password = std::string());
    UserInfo();
    UserInfo(const UserInfo & rhs);
    UserInfo& operator=(const UserInfo & rhs);
    int id;
    QString email;
    QString password;
    QString name;
    QString lastName;
    QString phoneNumber;
    QString picture;
};

#endif // USERINFO_H
