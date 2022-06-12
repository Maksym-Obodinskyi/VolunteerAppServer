#define LOG_CATEGORY    "User"
#define LOG_LEVEL       _TRACE_
#include "Logger.h"

#include "UserInfo.h"

//UserInfo::UserInfo(std::string _name
//         , std::string _lastName
//         , std::string _number
//         , std::string _photo
//         , double      _rating
//         , std::string _email
//         , std::string _password) :
//            name(_name)
//          , lastName(_lastName)
//          , number(_number)
//          , photo(_photo)
//          , rating(_rating)
//          , email(_email)
//          , password(_password)
//{
//    TRACE();
//}

UserInfo::UserInfo()
{

}

UserInfo::UserInfo(const UserInfo & rhs)
{
    id = rhs.id;
    email = rhs.email;
    password = rhs.password;
    name = rhs.name;
    lastName = rhs.lastName;
    phoneNumber= rhs.phoneNumber;
    picture = rhs.picture;
}

UserInfo& UserInfo::operator=(const UserInfo & rhs)
{
    id = rhs.id;
    email = rhs.email;
    password = rhs.password;
    name = rhs.name;
    lastName = rhs.lastName;
    phoneNumber= rhs.phoneNumber;
    picture = rhs.picture;
    return *this;
}
