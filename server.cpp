#define LOG_LEVEL _TRACE_
#define LOG_CATEGORY "Server"
#include "Logger.h"

#include "server.h"
#include "responce.h"
#include <QtDebug>
#include <QSqlError>
#include <iostream>
#include <charconv>

Server::Server(QObject *parent) : QTcpServer(parent)
{
    TRACE();
    connectToDB();
}

Server::~Server()
{
    TRACE();
    closeConnection();
}

void Server::Run()
{
    TRACE();
    this->listen(QHostAddress("127.0.0.1"), 4243); ////TODO: make dynamic generation of port
    connect(this, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void Server::onNewConnection()
{
    TRACE();
    QTcpSocket *clientSocket = this->nextPendingConnection();
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));
    sockets.push_back(clientSocket);
    for (QTcpSocket* socket : sockets) {
        socket->write("c");
    }
}

void Server::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    TRACE();
    if (socketState == QAbstractSocket::UnconnectedState)
    {
        QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
        sockets.removeOne(sender);
    }
}

void Server::onReadyRead()
{
    TRACE();
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray datas = sender->readAll();

    std::cout << "datas: " << datas.toStdString()<<std::endl;
    this->parseUserRequest(datas);
}

void Server::parseUserRequest(QByteArray &request)
{
    TRACE();
    if(request.isEmpty()) {
      return;
    }

    std::unique_ptr<Message> msg;

    int size = this->getRequestsSize(request);
    QRegExp reg("\\|(.+)");
    reg.indexIn(request);
    std::string body = reg.cap(1).toStdString();

    switch(request[0]){
        case 'l':
            msg.reset( new MessageLogIn(size, body));
            break;
        case 'g':
            msg.reset( new MessageGetRequest(size, body));
            break;
        case 'u':
            msg.reset( new MessageUpdateRequest(size, body));
            break;
        case 'p':
            msg.reset( new MessageUpdateProfile(size, body));
            break;
        case 'n':
            msg.reset( new MessageNewUser(size, body));
            break;
        case 'a':
            msg.reset( new MessageAddRequest(size, body));
            break;
        case 'r':
            msg.reset( new MessageRemoveRequest(size, body));
            break;
        case 'd':
            msg.reset( new MessageLogOut(size, body));
            break;
        default:
            WARNING("Unrecognized message type received - {}", request[0]);
            break;
    }
    msg->process();
    TRACE();
    auto answer = msg->sendToDB(Database);
    TRACE();
    sendRequestStatus(answer->serialize());
    TRACE();
}

int Server::getRequestsSize(const QByteArray& request)
{
    TRACE();
    QRegExp regExpr("\\w+:([0-9]+)");
    regExpr.indexIn(request);
    int size = regExpr.cap(1).toInt();
    return  size;
}

void Server::sendRequestStatus(const QByteArray& status)
{
    TRACE();
    DEBUG("answer : {}", status);
    for (QTcpSocket* socket : sockets) {
        socket->write(status);
    }
}
void Server::connectToDB()
{
    TRACE();
    Database = QSqlDatabase::addDatabase("QSQLITE", "SQLITE");

    Database.setDatabaseName("VolunteerApp.db");

    if (!Database.open())
    {
        std::cout << Database.lastError().text().toStdString()<<std::endl;
    }
    else {
        std::cout << "Database is opened successfuly!"<<std::endl;
    }
}

bool Server::closeConnection()
{
    TRACE();
    Database.close();
    return 0;
}

void Server::receiveUsersRequest()
{
    TRACE();
}

RequestInfo* Server::sendRequestInfo()
{
    TRACE();
    return nullptr;
}

UserInfo* Server::sendUsersInfo()
{
    TRACE();
    return nullptr;
}
