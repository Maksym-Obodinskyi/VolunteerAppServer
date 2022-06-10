#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QDateTime>
#include <QSqlQuery>

struct location
{
    double N;
    double E;
};

struct RequestInfo
{
    int id;
    int userId;
    location _location;
    QString description;
    QString title;
    QString categories;
    int date;
    int targetDate;
};

struct UserInfo
{
    int id;
    QString email;
    QString password;
    QString name;
    QString lastName;
    QString phoneNumber;
    QString picture;
    double rating;
};

class Message : public QObject
{
    Q_OBJECT
public:
    explicit Message(QObject *parent = nullptr);
    Message(int msgSize, std::string body);
    virtual void process();
    virtual int sendToDB(QSqlDatabase &Database);
    std::string getMessage() {return msg;}
    QStringList splitMessage();
signals:
private:
    std::string msg;
    int msgSize=0;
};

class MessageLogIn : public Message
{
    Q_OBJECT
public:
    explicit MessageLogIn (int size, std::string body);
    void process() override;
    int sendToDB(QSqlDatabase &Database) override;
    QString getPhoneNumber() {return requestBody.phoneNumber;}
    QString getPassword() {return requestBody.password;}
    void setPhoneNumber(QString log){requestBody.phoneNumber = log;}
    void setPassword(QString passw){requestBody.password = passw;}
signals:
private:
    struct _requestBody {
        QString phoneNumber;
        QString password;
    }requestBody;
};

class MessageLogOut : public Message
{
    Q_OBJECT
public:
    explicit MessageLogOut (int size, std::string body);
    void process() override;
    int sendToDB(QSqlDatabase &Database) override;
    void setUserName(QString user_name){userName = user_name;}
    QString getUserName() {return userName;}
signals:
private:
    QString userName;
};

class MessageNewUser : public Message
{
    Q_OBJECT
public:
    explicit MessageNewUser (int size, std::string body);
    void process() override;
    int sendToDB(QSqlDatabase &Database) override;
    UserInfo getUserInfo(){return userInfo;}
    void setUserInfo(int user_id,
                     QString user_login,
                     QString user_password,
                     QString user_name,
                     QString user_lastName,
                     QString user_phoneNumber,
                     QString user_picture,
                     double user_rating);
signals:
private:
    UserInfo userInfo;
};

class MessageUpdateProfile : public Message
{
    Q_OBJECT
public:
    explicit MessageUpdateProfile (int size, std::string body);
    void process() override;
    int sendToDB(QSqlDatabase &Database) override;
    UserInfo getUserInfo(){return userInfo;}
    void setUserInfo(int user_id,
                     QString user_login,
                     QString user_password,
                     QString user_name,
                     QString user_lastName,
                     QString user_phoneNumber,
                     QString user_picture,
                     double user_rating);
signals:
private:
    UserInfo userInfo;

};

class MessageAddRequest : public Message
{
    Q_OBJECT
public:
    explicit MessageAddRequest (int size, std::string body);
    void process() override;
    int sendToDB(QSqlDatabase &Database) override;
    RequestInfo getRequestInfo(){return requestInfo;}
    void setRequestInfo(int request_id,
                        int request_userId,
                        double request_location_e,
                        double request_location_n,
                        QString request_description,
                        QString request_title,
                        QString request_categories,
                        int request_date,
                        int request_targetDate);
signals:
private:
    RequestInfo requestInfo;
};

class MessageGetRequest : public Message
{
    Q_OBJECT
public:
    explicit MessageGetRequest (int size, std::string body);
    void process() override;
    int sendToDB(QSqlDatabase &Database) override;
    QString getFilter(){return filter;}
    void setFilter(QString filt){ filter = filt;}

signals:
private:
    QString filter;

};

class MessageUpdateRequest : public Message
{
    Q_OBJECT
public:
    explicit MessageUpdateRequest (int size, std::string body);
    void process() override;
    int sendToDB(QSqlDatabase &Database) override;
    RequestInfo getRequestInfo(){return requestInfo;}
    void setRequestInfo(int request_id,
                        int request_userId,
                        double request_location_e,
                        double request_location_n,
                        QString request_description,
                        QString request_title,
                        QString request_categories,
                        int request_date,
                        int request_targetDate);
signals:
private:
    RequestInfo requestInfo;

};

class MessageRemoveRequest : public Message
{
    Q_OBJECT
public:
    explicit MessageRemoveRequest (int size, std::string body);
    void process() override;
    int sendToDB(QSqlDatabase &Database) override;
    void setRequestID(int request){requestID = request;}
    int getRequestID() {return requestID;}
signals:
private:
    int requestID=0;
};
#endif // MESSAGE_H
