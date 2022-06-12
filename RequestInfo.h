#ifndef REQUESTINFO_H
#define REQUESTINFO_H

#include "UserInfo.h"

#include <string>
#include <set>
#include <QObject>

struct location
{
    double N;
    double E;
};

struct RequestInfo : public QObject
{
    Q_GADGET
public:
//    RequestInfo(std::pair<double, double> _location
//          , const std::string & _description
//          , const std::string & _title
//          , const std::string & _categories
//          , int _date);
    RequestInfo() : QObject(nullptr) {}
    RequestInfo(const RequestInfo & req);
    RequestInfo(RequestInfo && req);
    RequestInfo& operator=(const RequestInfo & req);
    RequestInfo& operator=(RequestInfo && req);

    static void declareInQML();

///////////////////////////////////////////////////////////////////
    Q_PROPERTY(int      id              READ getId          )
    Q_PROPERTY(QString  email           READ getEmail       )
    Q_PROPERTY(QString  password        READ getPassword    )
    Q_PROPERTY(QString  name            READ getName        )
    Q_PROPERTY(QString  lastName        READ getLastName    )
    Q_PROPERTY(QString  phone           READ getPhone       )
    Q_PROPERTY(QImage   picture         READ getPicture     )

    Q_PROPERTY(double   latitude        READ getLatitude    )
    Q_PROPERTY(double   longitude       READ getLongitude   )
    Q_PROPERTY(QString  description     READ getDescription )
    Q_PROPERTY(QString  title           READ getTitle       )
    Q_PROPERTY(QString  categories      READ getCategories  )
    Q_PROPERTY(int      date            READ getDate        )
    Q_PROPERTY(int      targetDate      READ getTargetDate  )

///////////////////////////////////////////////////////////////////
    int getId();
    QString getEmail();
    QString getPassword();
    QString getName();
    QString getLastName();
    QString getPhone();
    QImage getPicture();

    double getLatitude();
    double getLongitude();
    QString getDescription();
    QString getTitle();
    QString getCategories();
    int getDate();
    int getTargetDate();
///////////////////////////////////////////////////////////////////
    void setId(int var);
    void setEmail(QString var);
    void setPassword(QString var);
    void setName(QString var);
    void setLastName(QString var);
    void setPhone(QString var);
    void setPicture(QImage var);

    void setLatitude(double var);
    void setLongitude(double var);
    void setDescription(QString var);
    void setTitle(QString var);
    void setCategories(QString var);
    void setDate(int var);
    void setTargetDate(int var);
///////////////////////////////////////////////////////////////////

    int id;
    QString UserPhone;
    location _location;
    QString description;
    QString title;
    QString categories;
    int date;
    int targetDate;
    UserInfo userInfo;
};

Q_DECLARE_METATYPE(RequestInfo);

#endif // REQUESTINFO_H
