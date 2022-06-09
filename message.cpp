#define LOG_LEVEL _TRACE_
#define LOG_CATEGORY "Message"
#include "Logger.h"

#include "message.h"
#include <iostream>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>

Message::Message(QObject *parent) : QObject(parent)
{

}

Message::Message(int size, std::string body)
                : msg(body), msgSize(size)
{

}

void Message::process()
{
    std::cout<<"Message process"<<std::endl;
}
int Message::sendToDB([[maybe_unused]]QSqlDatabase &Database){
    return 0;
}
QStringList Message::splitMessage()
{
    return QString::fromStdString(getMessage()).split(QRegExp(":"), QString::SkipEmptyParts);
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
        setLogin(list.at(0));
        setPassword(list.at(1));
    }
}
int MessageLogIn::sendToDB(QSqlDatabase &Database){

    QSqlQuery query(Database);
    bool res = query.prepare("SELECT id FROM UserTable WHERE PhoneNumber = ? AND Password = ?");
    DEBUG("{}",res);
    query.bindValue(0, getLogin());
    query.bindValue(1, getPassword());

    if(query.exec()){
        DEBUG("query executed successfuly!");
        QSqlRecord record = query.record();
        while(record.next())
        return 0;
    }else{
        WARNING("{}",query.lastError().text().toStdString());
        return 1;
    }
    return 0;
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
int MessageLogOut::sendToDB(QSqlDatabase &Database){

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
    if(!list.empty() && list.size()==9)
    {
        setRequestInfo(list.at(0).toInt(),
                    list.at(1).toInt(),
                    list.at(2).toDouble(),
                    list.at(3).toDouble(),
                    list.at(4),
                    list.at(5),
                    list.at(6),
                    list.at(7).toInt(),
                    list.at(8).toInt());
    }
}

void MessageAddRequest::setRequestInfo(int request_id,
                                       int request_userId,
                                       double request_location_e,
                                       double request_location_n,
                                       QString request_description,
                                       QString request_title,
                                       QString request_categories,
                                       int request_date,
                                       int request_targetDate)
{
    requestInfo.id = request_id;
    requestInfo.userId = request_userId;
    requestInfo._location.E = request_location_e;
    requestInfo._location.N = request_location_n;
    requestInfo.description = request_description;
    requestInfo.title = request_title;
    requestInfo.categories = request_categories;
    requestInfo.date = request_date;
    requestInfo.targetDate = request_targetDate;
}

int MessageAddRequest::sendToDB(QSqlDatabase &Database){
    TRACE()
    QSqlQuery query(Database);
    bool res = query.prepare("INSERT INTO RequestTable (UserId, Title, Description, LocationE, LocationN, Date, TargetDate)"
                              "VALUES (:UserId, :Title, :Description, :LocationE, :LocationN, :Date, :TargetDate)");
    DEBUG("{}",res);
    query.bindValue(":UserId", getRequestInfo().userId);
    query.bindValue(":Title", getRequestInfo().title);
    query.bindValue(":Description", getRequestInfo().description);
    query.bindValue(":LocationE", getRequestInfo()._location.E);
    query.bindValue(":LocationN", getRequestInfo()._location.N);
    query.bindValue(":Date", getRequestInfo().date);
    query.bindValue(":TargetDate", getRequestInfo().targetDate);
    if(query.exec()){
        DEBUG("query executed successfuly!");
        return 0;
    }else{
        WARNING("{}",query.lastError().text().toStdString());
        return 1;
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

int MessageRemoveRequest::sendToDB(QSqlDatabase &Database){

    QSqlQuery query(Database);
    bool res = query.prepare("DELETE FROM RequestTable WHERE id = ?");
    DEBUG("{}",res);
    query.addBindValue(getRequestID());

    if(query.exec()){
        DEBUG("query executed successfuly!");
        return 0;
    }else{
        WARNING("{}",query.lastError().text().toStdString());
        return 1;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////todo
MessageGetRequest::MessageGetRequest(int size, std::string body) : Message (size, body)
{

}

void MessageGetRequest::process()
{
    std::cout<<"MessageGetRequest process"<<std::endl;

}

int MessageGetRequest::sendToDB(QSqlDatabase &Database){
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////
MessageNewUser::MessageNewUser(int size, std::string body) : Message (size, body)
{

}
void MessageNewUser::setUserInfo(int user_id,
                                 QString user_email,
                                 QString user_password,
                                 QString user_name,
                                 QString user_lastName,
                                 QString user_phoneNumber,
                                 QString user_picture,
                                 double user_rating)
{
    userInfo.id = user_id;
    userInfo.name = user_name;
    userInfo.email = user_email;
    userInfo.password = user_password;
    userInfo.rating = user_rating;
    userInfo.picture = user_picture;
    userInfo.lastName = user_lastName;
    userInfo.phoneNumber = user_phoneNumber;
    std::cout<<"setUserInfo"<<std::endl;
}
void MessageNewUser::process()
{
    std::cout<<"MessageNewUser process"<<std::endl;
    QStringList list = splitMessage();
    if(!list.empty() && list.size()==8)
    {
        setUserInfo(list.at(0).toInt(),
                    list.at(1),
                    list.at(2),
                    list.at(3),
                    list.at(4),
                    list.at(5),
                    list.at(6),
                    list.at(7).toDouble());
       std::cout<< getUserInfo().name.toStdString()<<std::endl;
    }
    std::cout<<"MessageNewUser process end"<<std::endl;
}
int MessageNewUser::sendToDB(QSqlDatabase &Database){

    TRACE()
    QSqlQuery query(Database);
    bool res = query.prepare("INSERT INTO UserTable (PhoneNumber, Password, Name, LastName, Picture, Rating, Email) VALUES (:PhoneNumber, :Password, :Name, :LastName, :Picture, :Rating, :Email)");
    DEBUG("{}",res);
    query.bindValue(":PhoneNumber", getUserInfo().phoneNumber);
    query.bindValue(":Password", getUserInfo().password);
    query.bindValue(":Name", getUserInfo().name);
    query.bindValue(":LastName", getUserInfo().lastName);
    query.bindValue(":Picture", getUserInfo().picture);
    query.bindValue(":Rating", getUserInfo().rating);
    query.bindValue(":Email", getUserInfo().email);
    if(query.exec()){
        DEBUG("query executed successfuly!");
        return 0;
    }else{
        WARNING("{}",query.lastError().text().toStdString());
        return 1;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////
MessageUpdateProfile::MessageUpdateProfile(int size, std::string body) : Message (size, body)
{

}
void MessageUpdateProfile::setUserInfo(int user_id,
                                 QString user_email,
                                 QString user_password,
                                 QString user_name,
                                 QString user_lastName,
                                 QString user_phoneNumber,
                                 QString user_picture,
                                 double user_rating)
{
    userInfo.id = user_id;
    userInfo.name = user_name;
    userInfo.email = user_email;
    userInfo.password = user_password;
    userInfo.rating = user_rating;
    userInfo.picture = user_picture;
    userInfo.lastName = user_lastName;
    userInfo.phoneNumber = user_phoneNumber;
}
void MessageUpdateProfile::process()
{
    std::cout<<"MessageUpdateProfile process"<<std::endl;
    QStringList list = splitMessage();
    if(!list.empty() && list.size()==8)
    {
        setUserInfo(list.at(0).toInt(),
                    list.at(1),
                    list.at(2),
                    list.at(3),
                    list.at(4),
                    list.at(5),
                    list.at(6),
                    list.at(7).toDouble());
    }

}
int MessageUpdateProfile::sendToDB(QSqlDatabase &Database){
    return 0;
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
                    list.at(1).toInt(),
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
                                       int request_userId,
                                       double request_location_e,
                                       double request_location_n,
                                       QString request_description,
                                       QString request_title,
                                       QString request_categories,
                                       int request_date,
                                       int request_targetDate)
{
    requestInfo.id = request_id;
    requestInfo.userId = request_userId;
    requestInfo._location.E = request_location_e;
    requestInfo._location.N = request_location_n;
    requestInfo.description = request_description;
    requestInfo.title = request_title;
    requestInfo.categories = request_categories;
    requestInfo.date = request_date;
    requestInfo.targetDate = request_targetDate;
}

int MessageUpdateRequest::sendToDB(QSqlDatabase &Database){
    return 0;
}
