#ifndef RESPONCE_H
#define RESPONCE_H

#include <iostream>
#include <QByteArray>
#include <QString>
#include <QVector>
#include <QStringList>
#include <QRegExp>
#include <charconv>
#include "UserInfo.h"
#include "RequestInfo.h"

struct Responce {
    char type;
    int err;

    explicit Responce(char _type = '|') : type(_type) {}

    virtual QByteArray serialize()
    {
        QByteArray ret;
        ret += type;
        ret += ':';
        char arr[5];
        auto [ptr, ec] = std::to_chars(arr, arr + 5, err);
        if (ec == std::errc()) {
            for (auto symbol = arr; symbol != ptr; symbol++) {
                ret += *symbol;
            }
        } else {
            ret += '9';
        }
        return ret;
    }

    virtual void deserialize(QByteArray arr)
    {
        std::cout << "deserialize - " << arr.constData() << std::endl;
        if (arr.size() >= 1) {
            int result;
            const auto[_, ec] = std::from_chars(arr.constData(), arr.constData() + arr.size(), result);

            if (ec == std::errc()) {
                err = result;
            } else {
                err = 1000;
            }
        } else {
            err = 1001;
        }
    }
    QByteArray addItem(QString itemsField)
    {
        return itemsField.toUtf8() + ":";
    }
    QByteArray addItem(int itemsField)
    {
        return QString::number(itemsField).toUtf8() + ":";
    }
};


struct LogInResponce : Responce {
    LogInResponce() : Responce('l') {}
    UserInfo userInfo;
    QByteArray serialize() override
    {
        QByteArray ret;
        ret += type;
        ret += ':';
        char arr[5];
        auto [ptr, ec] = std::to_chars(arr, arr + 5, err);
        if (ec == std::errc()) {
            for (auto symbol = arr; symbol != ptr; symbol++) {
                ret += *symbol;
            }
        } else {
            ret += '9';
            return ret;
        }
        ret += '|';

        ret += addItem(userInfo.id);
        ret += addItem(userInfo.name);
        ret += addItem(userInfo.lastName);
        ret += addItem(userInfo.email);
        ret += addItem(userInfo.phoneNumber);
        ret += userInfo.picture + ";";

        std::cout<< "ret: "<< ret.constData() << std::endl;
        return ret;
    }
     void deserialize(QByteArray arr) override
    {
         std::cout << "deserialize - " << arr.constData() << std::endl;
         if (arr.size() >= 1) {
             arr.remove(0, arr.indexOf('|')+1);
             std::cout << "arr - " << arr.toStdString() << std::endl;

             UserInfo userInfo;
             QList<QByteArray> InfoList =  arr.split(':');
             if(InfoList.size() == 6)
             {
                 userInfo.id = InfoList[0].toInt();
                 userInfo.name = InfoList[1];
                 userInfo.lastName = InfoList[2];
                 userInfo.email = InfoList[3];
                 userInfo.phoneNumber = InfoList[4];
                 userInfo.picture = InfoList[5];
             }
             err = 0;
         } else {
             err = 1001;
         }
    }
};

struct LogOutResponce : Responce {
    LogOutResponce() : Responce('d') {}
};

struct NewUserResponce : Responce {
    NewUserResponce() : Responce('n') {}
};

struct UpdateProfileResponce : Responce {
    UpdateProfileResponce() : Responce('p') {}
};

struct AddRequestResponce : Responce {
    AddRequestResponce() : Responce('a') {}
};
/////////////////////////////////////////////////////////////
/// \brief The GetRequestResponce struct
/////////////////////////////////////////////////////////////
struct GetRequestResponce : Responce {
    int err;
    QVector<RequestInfo> requestsList;
    GetRequestResponce() : Responce('g') {}
    QByteArray serialize() override
    {
        QByteArray ret;
        ret += type;
        ret += ':';
        char arr[5];
        auto [ptr, ec] = std::to_chars(arr, arr + 5, err);
        if (ec == std::errc()) {
            for (auto symbol = arr; symbol != ptr; symbol++) {
                ret += *symbol;
            }
        } else {
            ret += '9';
            return ret;
        }
        ret += '|';
        for( const auto &item : requestsList)
        {
            ret += addItem(item.id);
            ret += addItem(item.title);
            ret += addItem(item.description);
            ret += addItem(item._location.E);
            ret += addItem(item._location.N);
            ret += addItem(item.date);
            ret += addItem(item.categories);
            ret += addItem(item.targetDate);
            ret += addItem(item.userInfo.name);
            ret += addItem(item.userInfo.lastName);
            ret += addItem(item.userInfo.email);
            ret += addItem(item.userInfo.phoneNumber);

            ret += item.userInfo.picture + ";";
        }
        std::cout<< "ret: "<< ret.constData() << std::endl;
        return ret;
    }
     void deserialize(QByteArray arr) override
    {
         std::cout << "deserialize - " << arr.constData() << std::endl;
         if (arr.size() >= 1) {
             arr.remove(0, arr.indexOf('|')+1);
             std::cout << "arr - " << arr.toStdString() << std::endl;

             QList<QByteArray> reqList =  arr.split(';');
             for(auto req : reqList)
             {
                 RequestInfo reqInfo;
                 std::cout << "req - " << req.toStdString() << std::endl;
                 QList<QByteArray> InfoList =  req.split(':');
                 if(InfoList.size() > 1)
                 {
                     reqInfo.id = InfoList[0].toInt();
                     reqInfo.title = InfoList[1];
                     reqInfo.description = InfoList[2];
                     reqInfo._location.E = InfoList[3].toDouble();
                     reqInfo._location.N = InfoList[4].toDouble();
                     reqInfo.date = InfoList[5].toInt();
                     reqInfo.categories = InfoList[6];
                     reqInfo.targetDate = InfoList[7].toInt();
                     reqInfo.userInfo.name = InfoList[8];
                     reqInfo.userInfo.lastName = InfoList[9];
                     reqInfo.userInfo.email = InfoList[10];
                     reqInfo.userInfo.phoneNumber = InfoList[11];
                     reqInfo.userInfo.picture = InfoList[12];

                     requestsList.append(reqInfo);
                 }
             }
             err = 0;
         } else {
             err = 1001;
         }
    }
};
/////////////////////////////////////////////////////////////
/// \TODO: Finish GetResponce serialize
/////////////////////////////////////////////////////////////

struct UpdateRequestResponce : Responce {
    UpdateRequestResponce() : Responce('u') {}
};

struct RemoveRequestResponce : Responce {
    RemoveRequestResponce() : Responce('r') {}
};

#endif // RESPONCE_H
