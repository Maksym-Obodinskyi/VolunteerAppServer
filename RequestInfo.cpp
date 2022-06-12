#define LOG_CATEGORY "RequestInfo"
#define LOG_LEVEL _TRACE_
#include "Logger.h"
#include "RequestInfo.h"

#include <QQmlEngine>

//RequestInfo::RequestInfo(std::pair<double, double> _location
//                 , const std::string & _description
//                 , const std::string & _title
//                 , const std::string & _categories
//                 , int _date) :
//                   QObject(nullptr)
//                 , location(_location)
//                 , description(_description)
//                 , title(_title)
//                 , categories(_categories)
//                 , date(_date)
//{
//}

RequestInfo::RequestInfo(const RequestInfo & req)
{
    TRACE();
    id = req.id;
    UserPhone = req.UserPhone;
    _location = req._location;
    description = req.description;
    title = req.title;
    categories = req.categories;
    date = req.date;
    targetDate = req.targetDate;
    userInfo = req.userInfo;
}

RequestInfo& RequestInfo::operator=(const RequestInfo & req)
{
    TRACE();
    id = req.id;
    UserPhone = req.UserPhone;
    _location = req._location;
    description = req.description;
    title = req.title;
    categories = req.categories;
    date = req.date;
    targetDate = req.targetDate;
    userInfo = req.userInfo;
    return *this;
}

RequestInfo::RequestInfo(RequestInfo && req)
{
    TRACE();
    id = req.id;
    UserPhone = std::move(req.UserPhone);
    _location = req._location;
    description = std::move(req.description);
    title = std::move(req.title);
    categories = std::move(req.categories);
    date = req.date;
    targetDate = req.targetDate;
    userInfo = std::move(req.userInfo);
}

RequestInfo& RequestInfo::operator=(RequestInfo && req)
{
    TRACE();
    id = req.id;
    UserPhone = std::move(req.UserPhone);
    _location = req._location;
    description = std::move(req.description);
    title = std::move(req.title);
    categories = std::move(req.categories);
    date = req.date;
    targetDate = req.targetDate;
    userInfo = std::move(req.userInfo);
    return *this;
}

void RequestInfo::declareInQML()
{
    TRACE();
    qmlRegisterType<RequestInfo>("request", 1, 0, "Request");
}

int RequestInfo::getId()
{
    return userInfo.id;
}

QString RequestInfo::getPassword()
{
    return userInfo.password;
}

QString RequestInfo::getEmail()
{
    return userInfo.email;
}

QString RequestInfo::getName()
{
    return userInfo.name;
}

QString RequestInfo::getLastName()
{
    return userInfo.lastName;
}

QString RequestInfo::getPhone()
{
    return userInfo.phoneNumber;
}

QString RequestInfo::getPicture()
{
    return userInfo.picture;
}

double RequestInfo::getLatitude()
{
    return _location.E;
}

double RequestInfo::getLongitude()
{
    return _location.N;
}

QString RequestInfo::getDescription()
{
    return description;
}

QString RequestInfo::getTitle()
{
    return title;
}

QString RequestInfo::getCategories()
{
    return categories;
}

int     RequestInfo::getDate()
{
    return date;
}

int     RequestInfo::getTargetDate()
{
    return targetDate;
}


/////////////////////////////////////////////////////////////////////////


void RequestInfo::setId(int var)
{
    userInfo.id = var;

}

void RequestInfo::setEmail(QString var)
{
    userInfo.email = var;

}

void RequestInfo::setPassword(QString var)
{
    userInfo.password = var;

}

void RequestInfo::setName(QString var)
{
    userInfo.name = var;

}

void RequestInfo::setLastName(QString var)
{
    userInfo.lastName = var;

}

void RequestInfo::setPhone(QString var)
{
    userInfo.phoneNumber = var;

}

void RequestInfo::setPicture(QString var)
{
    userInfo.picture = var;

}

void RequestInfo::setLatitude(double var)
{
    _location.E = var;

}

void RequestInfo::setLongitude(double var)
{
    _location.N = var;

}

void RequestInfo::setDescription(QString var)
{
    description = var;

}

void RequestInfo::setTitle(QString var)
{
    title = var;

}

void RequestInfo::setCategories(QString var)
{
    categories = var;

}

void RequestInfo::setDate(int var)
{
    date = var;

}

void RequestInfo::setTargetDate(int var)
{
    targetDate = var;

}
