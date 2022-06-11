#define LOG_LEVEL _TRACE_
#define LOG_CATEGORY "Message"
#include "Logger.h"

#include "message.h"
#include <iostream>
#include <utility>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QVector>

Message::Message(QObject *parent) : QObject(parent)
{

}
Message::~Message()
{
    TRACE();
}
Message::Message(int size, std::string body)
                : msg(body), msgSize(size)
{

}

void Message::process()
{
    std::cout<<"Message process"<<std::endl;
}

std::unique_ptr<Responce> Message::sendToDB([[maybe_unused]]QSqlDatabase &Database){
    return 0;
}

QStringList Message::splitMessage()
{
    return QString::fromStdString(getMessage()).split(QRegExp(":"));
}

///////////////////////////////////////////////////////////////////////////////////////////
MessageLogIn::MessageLogIn(int size, std::string body) : Message (size, body)
{

}

void MessageLogIn::process()
{
    std::cout<<"MessageLogIn process"<<std::endl;
    QStringList list = splitMessage();
    if(!list.empty())
    {
        setPhoneNumber(list.at(0));
        setPassword(list.at(1));
    }
}

std::unique_ptr<Responce> MessageLogIn::sendToDB(QSqlDatabase &Database){

    QSqlQuery query(Database);
    bool res = query.prepare("SELECT id FROM UserTable WHERE PhoneNumber = ? AND Password = ?");
    DEBUG("{}",res);
    query.bindValue(0, getPhoneNumber());
    query.bindValue(1, getPassword());

    std::unique_ptr<Responce> ptr(new LogInResponce);
    if(query.exec() && query.next()){
        DEBUG("query executed successfuly!");
        QSqlRecord record = query.record();
        DEBUG("{}", query.value(record.indexOf("id")).toString().toStdString());
        ptr->err = 0;
        return ptr;
    }else{
        WARNING("{}", query.lastError().text().toStdString());
        ptr->err = 1;
        return ptr;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
MessageLogOut::MessageLogOut(int size, std::string body) : Message (size, body)
{

}

void MessageLogOut::process()
{
    std::cout<<"MessageLogOut process"<<std::endl;
    setUserName(QString::fromStdString(getMessage()));
}

std::unique_ptr<Responce> MessageLogOut::sendToDB(QSqlDatabase &Database){

    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////
MessageAddRequest::MessageAddRequest(int size, std::string body) : Message (size, body)
{

}

void MessageAddRequest::process()
{
    std::cout<<"MessageAddRequest process"<<std::endl;
    QStringList list = splitMessage();
    if(!list.empty() && list.size()==8)
    {
        setRequestInfo( list.at(0),
                        list.at(1).toDouble(),
                        list.at(2).toDouble(),
                        list.at(3),
                        list.at(4),
                        list.at(5),
                        list.at(6).toInt(),
                        list.at(7).toInt());
    }
}

void MessageAddRequest::setRequestInfo(QString UserPhone,
                                       double request_location_e,
                                       double request_location_n,
                                       QString request_description,
                                       QString request_title,
                                       QString request_categories,
                                       int request_date,
                                       int request_targetDate)
{
    requestInfo.UserPhone = UserPhone;
    requestInfo._location.E = request_location_e;
    requestInfo._location.N = request_location_n;
    requestInfo.description = request_description;
    requestInfo.title = request_title;
    requestInfo.categories = request_categories;
    requestInfo.date = request_date;
    requestInfo.targetDate = request_targetDate;
}

std::unique_ptr<Responce> MessageAddRequest::sendToDB(QSqlDatabase &Database){
    TRACE()
    QSqlQuery query(Database);
    std::unique_ptr<Responce> ptr(new AddRequestResponce);
    DEBUG("User phone: {}", getRequestInfo().UserPhone.toStdString())
    bool res =query.prepare("SELECT id FROM UserTable WHERE PhoneNumber = ?");
    query.addBindValue(getRequestInfo().UserPhone);
    int UserId =0;
    if (query.exec()) {
        if(query.next()){
            QSqlRecord record = query.record();
            UserId = query.value(record.indexOf("id")).toInt();
            DEBUG("User id: {}", query.value(record.indexOf("id")).toString().toStdString());}
    } else {
            WARNING("User with this phone number is not found")
            ptr->err = 1;
            return ptr;
    }

    res = query.prepare("INSERT INTO RequestTable (UserId, Title, Description, LocationE, LocationN, Category, Date, TargetDate)"
                              "VALUES (:UserId, :Title, :Description, :LocationE, :LocationN, :Category, :Date, :TargetDate)");
    DEBUG("{}",res);
    query.bindValue(":UserId", UserId);
    query.bindValue(":Title", getRequestInfo().title);
    query.bindValue(":Description", getRequestInfo().description);
    query.bindValue(":LocationE", getRequestInfo()._location.E);
    query.bindValue(":LocationN", getRequestInfo()._location.N);
    query.bindValue(":Category", getRequestInfo().categories);
    query.bindValue(":Date", getRequestInfo().date);
    query.bindValue(":TargetDate", getRequestInfo().targetDate);

    if(query.exec()){
        DEBUG("query executed successfuly!");
        ptr->err = 0;
        return ptr;
    } else {
        WARNING("{}",query.lastError().text().toStdString());
        ptr->err = 1;
        return ptr;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
MessageRemoveRequest::MessageRemoveRequest(int size, std::string body) : Message (size, body)
{

}

void MessageRemoveRequest::process()
{
    std::cout<<"MessageRemoveRequest process"<<std::endl;
    setRequestID(std::stoi(getMessage()));
}

std::unique_ptr<Responce> MessageRemoveRequest::sendToDB(QSqlDatabase &Database){

    QSqlQuery query(Database);
    bool res = query.prepare("DELETE FROM RequestTable WHERE id = ?");
    DEBUG("{}",res);
    query.addBindValue(getRequestID());
    std::unique_ptr<Responce> ptr(new RemoveRequestResponce);
    if(query.exec()){
        DEBUG("query executed successfuly!");
        ptr->err = 0;
        return ptr;
    }else{
        WARNING("{}",query.lastError().text().toStdString());
        ptr->err = 1;
        return ptr;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////todooo
MessageGetRequest::MessageGetRequest(int size, std::string body) : Message (size, body)
{

}

void MessageGetRequest::process()
{
    std::cout<<"MessageGetRequest process"<<std::endl;
    setFilter(getMessage().c_str());
    std::cout<< getFilter().toStdString()<<std::endl;
}

std::unique_ptr<Responce> MessageGetRequest::sendToDB(QSqlDatabase &Database){

    QSqlQuery query(Database);
    bool res ;
    res = query.prepare("SELECT * FROM RequestTable WHERE Category = ?");

    DEBUG("{}",res);
    query.bindValue(0, getFilter());
    GetRequestResponce* ptr = new GetRequestResponce;
    if (query.exec()) {
        DEBUG("query executed successfuly!");
        QString respond;
        while (query.next())
        {
            RequestInfo request;
            QSqlRecord record = query.record();
            request.id = query.value(record.indexOf("id")).toInt();
            request.userInfo.id = query.value(record.indexOf("UserId")).toInt();
            request.date = query.value(record.indexOf("Date")).toInt();
            request.title = query.value(record.indexOf("Title")).toString();
            request._location.E = query.value(record.indexOf("LocationE")).toDouble();
            request._location.N = query.value(record.indexOf("LocationN")).toDouble();
            request.categories = query.value(record.indexOf("Category")).toString();
            request.targetDate = query.value(record.indexOf("TargetDate")).toInt();
            request.description = query.value(record.indexOf("Description")).toString();

            QSqlQuery queryUser(Database);
            res = queryUser.prepare("SELECT * FROM UserTable WHERE id = ?");
            DEBUG("{}",res);
            queryUser.bindValue(0, QString::number(request.userInfo.id));
            if(queryUser.exec() && queryUser.next()){
                record = queryUser.record();
                request.userInfo.name = queryUser.value(record.indexOf("Name")).toString();
                request.userInfo.lastName = queryUser.value(record.indexOf("LastName")).toString();
                request.userInfo.email = queryUser.value(record.indexOf("Email")).toString();
                request.userInfo.phoneNumber = queryUser.value(record.indexOf("PhoneNumber")).toString();
                request.userInfo.picture = queryUser.value(record.indexOf("Picture")).toString();
            }
            ptr->requestsList.append(request);
        }
        ptr->err = 0;
    } else {
        WARNING("{}", query.lastError().text().toStdString());
        ptr->err = 1;
    }
    return std::unique_ptr<Responce>(ptr);
}
//////////////////////////////////////////////////////////////////////////////////////////////
MessageNewUser::MessageNewUser(int size, std::string body) : Message (size, body)
{

}

void MessageNewUser::setUserInfo(QString user_email,
                                 QString user_password,
                                 QString user_name,
                                 QString user_lastName,
                                 QString user_phoneNumber,
                                 QString user_picture)
{
    userInfo.name = user_name;
    userInfo.email = user_email;
    userInfo.password = user_password;
    userInfo.picture = user_picture;
    userInfo.lastName = user_lastName;
    userInfo.phoneNumber = user_phoneNumber;
    std::cout<<"setUserInfo"<<std::endl;
}

void MessageNewUser::process()
{
    std::cout<<"MessageNewUser process"<<std::endl;
    QStringList list = splitMessage();
    if(!list.empty() && list.size()==6)
    {
        setUserInfo(list.at(0),
                    list.at(1),
                    list.at(2),
                    list.at(3),
                    list.at(4),
                    list.at(5));
       std::cout<< getUserInfo().name.toStdString()<<std::endl;
    }
}

std::unique_ptr<Responce> MessageNewUser::sendToDB(QSqlDatabase &Database){

    TRACE()
    QSqlQuery query(Database);
    bool res = query.prepare("INSERT INTO UserTable (PhoneNumber, Password, Name, LastName, Picture, Email) VALUES (:PhoneNumber, :Password, :Name, :LastName, :Picture, :Email)");
    DEBUG("{}",res);
    query.bindValue(":PhoneNumber", getUserInfo().phoneNumber);
    query.bindValue(":Password", getUserInfo().password);
    query.bindValue(":Name", getUserInfo().name);
    query.bindValue(":LastName", getUserInfo().lastName);
    query.bindValue(":Picture", getUserInfo().picture);
    query.bindValue(":Email", getUserInfo().email);
    std::unique_ptr<Responce> ptr(new NewUserResponce);
    if(query.exec()){
        DEBUG("query executed successfuly!");
        ptr->err = 0;
        return ptr;
    }else{
        WARNING("{}",query.lastError().text().toStdString());
        ptr->err = 1;
        return ptr;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////
MessageUpdateProfile::MessageUpdateProfile(int size, std::string body) : Message (size, body)
{

}

void MessageUpdateProfile::setUserInfo(QString user_email,
                                     QString user_password,
                                     QString user_name,
                                     QString user_lastName,
                                     QString user_phoneNumber,
                                     QString user_picture)
{
    userInfo.name = user_name;
    userInfo.email = user_email;
    userInfo.password = user_password;
    userInfo.picture = user_picture;
    userInfo.lastName = user_lastName;
    userInfo.phoneNumber = user_phoneNumber;
}

void MessageUpdateProfile::process()
{
    std::cout<<"MessageUpdateProfile process"<<std::endl;
    QStringList list = splitMessage();
    if(!list.empty() && list.size()==7)
    {
        setUserInfo(list.at(0),
                    list.at(1),
                    list.at(2),
                    list.at(3),
                    list.at(4),
                    list.at(5));
    }
}

std::unique_ptr<Responce> MessageUpdateProfile::sendToDB(QSqlDatabase &Database){

    QSqlQuery query(Database);
    bool res = query.prepare("UPDATE UserTable SET Email=:email, Password=:password, Name=:name, LastName=:lastName,"
                             " Picture=:picture WHERE PhoneNumber=:phone");
    DEBUG("{}",res);
    query.bindValue(":email", getUserInfo().email);
    query.bindValue(":password", getUserInfo().password);
    query.bindValue(":name", getUserInfo().name);
    query.bindValue(":lastName", getUserInfo().lastName);
    query.bindValue(":picture", getUserInfo().picture);
    query.bindValue(":phone", getUserInfo().phoneNumber);
    std::unique_ptr<Responce> ptr(new UpdateProfileResponce);
    if(query.exec()){
        DEBUG("query executed successfuly!");
        ptr->err = 0;
        return ptr;
    }else{
        WARNING("{}",query.lastError().text().toStdString());
        ptr->err = 1;
        return ptr;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////
MessageUpdateRequest::MessageUpdateRequest(int size, std::string body) : Message (size, body)
{

}

void MessageUpdateRequest::process()
{
    std::cout<<"MessageUpdateRequest process"<<std::endl;
    QStringList list = splitMessage();
    if(!list.empty() && list.size()==9)
    {
        setRequestInfo(list.at(0).toInt(),
                    list.at(1),
                    list.at(2).toDouble(),
                    list.at(3).toDouble(),
                    list.at(4),
                    list.at(5),
                    list.at(6),
                    list.at(7).toInt(),
                    list.at(8).toInt());
    }
}

void MessageUpdateRequest::setRequestInfo(int request_id,
                                       QString request_userphone,
                                       double request_location_e,
                                       double request_location_n,
                                       QString request_description,
                                       QString request_title,
                                       QString request_categories,
                                       int request_date,
                                       int request_targetDate)
{
    requestInfo.id = request_id;
    requestInfo.UserPhone = request_userphone;
    requestInfo._location.E = request_location_e;
    requestInfo._location.N = request_location_n;
    requestInfo.description = request_description;
    requestInfo.title = request_title;
    requestInfo.categories = request_categories;
    requestInfo.date = request_date;
    requestInfo.targetDate = request_targetDate;
}

std::unique_ptr<Responce> MessageUpdateRequest::sendToDB(QSqlDatabase &Database){ ///////////////////if we have request id?????
    QSqlQuery query(Database);
    std::unique_ptr<Responce> ptr(new UpdateRequestResponce);
    bool res =query.prepare("SELECT id FROM UserTable WHERE PhoneNumber = ?");
    query.addBindValue(getRequestInfo().UserPhone);
    int UserId =0;
    if(query.exec() && query.next()){
            QSqlRecord record = query.record();
            UserId = query.value(record.indexOf("id")).toInt();
            DEBUG("User id: {}", query.value(record.indexOf("id")).toString().toStdString());
    } else {            WARNING("User with this phone number is not found")
            ptr->err = 1;
            return ptr;
    }

    res = query.prepare("UPDATE RequestTable SET UserId=:userId, LocationE=:locationE, LocationN=:locationN, Description=:description,"
                             " Title=:title, Category=:category, Date=:date, TargetDate=:targetDate WHERE id=:id");
    DEBUG("{}",res);
    query.bindValue(":userId", UserId);
    query.bindValue(":locationE", getRequestInfo()._location.E);
    query.bindValue(":locationN", getRequestInfo()._location.N);
    query.bindValue(":description", getRequestInfo().description);
    query.bindValue(":title", getRequestInfo().title);
    query.bindValue(":category", getRequestInfo().categories);
    query.bindValue(":date", getRequestInfo().date);
    query.bindValue(":targetDate", getRequestInfo().targetDate);

    if(query.exec()){
        DEBUG("query executed successfuly!");
        ptr->err = 0;
        return ptr;
    }else{
        WARNING("{}",query.lastError().text().toStdString());
        ptr->err = 1;
        return ptr;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////
QByteArray Message::serialize()
{
    TRACE();
    return QByteArray();
}


QByteArray MessageLogIn::serialize()
{
    TRACE();
    QByteArray ret;
    ret += 'l';
    ret += ':';
    char res[64];
    [[maybe_unused]] auto [ptr, ec] = std::to_chars(res, res + 64, requestBody.phoneNumber.size() + requestBody.password.size() + 1);
    ret.append(res, ptr - res);
    ret += '|';
    ret += requestBody.phoneNumber.toUtf8();
    ret += ':';
    ret += requestBody.password.toUtf8();
    return ret;
}


QByteArray MessageLogOut::serialize()
{
    TRACE();
    return QByteArray();
}

QByteArray MessageNewUser::serialize()
{
    TRACE();
    return QByteArray();
}

QByteArray MessageUpdateProfile::serialize()
{
    TRACE();
    return QByteArray();
}

QByteArray MessageAddRequest::serialize()
{
    TRACE();
    return QByteArray();
}

QByteArray MessageGetRequest::serialize()
{
    TRACE();
    return QByteArray();
}

QByteArray MessageUpdateRequest::serialize()
{
    TRACE();
    return QByteArray();
}

QByteArray MessageRemoveRequest::serialize()
{
    TRACE();
    return QByteArray();
}
