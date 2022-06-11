#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "message.h"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    ~Server();
    RequestInfo* sendRequestInfo();
    UserInfo* sendUsersInfo();
    void receiveUsersRequest();

    void Run();

public slots:
    void onNewConnection();
    bool closeConnection();
    void onReadyRead();
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);
signals:
private:
    void connectToDB();
    QString getUserCredentialsDB();
    void parseUserRequest(QByteArray& request);
    void editUserInfoDB(UserInfo* info);
    void addNewUserDB();
    void deleteUserDB();
    void addNewRequestDB();
    void editExistedRequestDB();
    void setRatingDB(int UsersID);
    RequestInfo* getRequesDB();
    void sendRequestStatus(const QByteArray& status);
    char convertFromBoolToChar(bool value);

    int getRequestsSize(const QByteArray& request);
    QList<QTcpSocket*> sockets;
    QSqlDatabase Database;
};

#endif // SERVER_H
