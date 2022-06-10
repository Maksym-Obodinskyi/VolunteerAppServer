#define LOG_LEVEL _TRACE_
#define LOG_CATEGORY "Message"
#include "Logger.h"
#include "server.h"
#include "signal.h"
#include <QtDebug>
#include <QSqlError>
#include <iostream>
#include <charconv>

Server::Server(QObject *parent) : QTcpServer(parent)
{

}

Server::~Server()
{
    delete msg;
}

void Server::Run()
{
    std::cout << "Server Run"<<std::endl;
    this->listen(QHostAddress("127.0.0.1"), 4243); ////TODO: make dynamic generation of port
    connect(this, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void Server::onNewConnection()
{
    std::cout << "onNewConnection"<<std::endl;
    QTcpSocket *clientSocket = this->nextPendingConnection();
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));
    sockets.push_back(clientSocket);
    for (QTcpSocket* socket : sockets) {
        socket->write(QByteArray::fromStdString(clientSocket->peerAddress().toString().toStdString() + " connected to server !\n"));
    }
}

void Server::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState)
    {
        QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
        sockets.removeOne(sender);
    }
}

void Server::onReadyRead()
{
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray datas = sender->readAll();

    std::cout << "onReadyRead"<<std::endl;
    std::cout << "datas: " << datas.toStdString()<<std::endl;
    this->parseUserRequest(datas);
}

void Server::parseUserRequest(QByteArray &request)
{
    if(request.isEmpty()) {
      return;
    }

    int size = this->getRequestsSize(request);
    QRegExp reg("\\|(.+)");
    reg.indexIn(request);
    std::string body = reg.cap(1).toStdString();

    switch(request[0]){
        case 'l':
            msg = new MessageLogIn(size, body);
            break;
        case 'g':
            msg = new MessageGetRequest(size, body);
            break;
        case 'u':
            msg = new MessageUpdateRequest(size, body);
            break;
        case 'p':
            msg = new MessageUpdateProfile(size, body);
            break;
        case 'n':
            msg = new MessageNewUser(size, body);
            break;
        case 'a':
            msg = new MessageAddRequest(size, body);
            break;
        case 'r':
            msg = new MessageRemoveRequest(size, body);
            break;
        case 'd':
            msg = new MessageLogOut(size, body);
            break;
        default:
            break;
    }
    msg->process();
    connectToDB();
    QString answer;
    answer += request.constData()[0];
    answer += ":";
    answer += msg->sendToDB(Database);
    DEBUG("answer: {}", answer.toStdString());
    sendRequestStatus(answer.toStdString().c_str());
}

int Server::getRequestsSize(const QByteArray& request)
{
    QRegExp regExpr("\\w+:([0-9]+)");
    regExpr.indexIn(request);
    int size = regExpr.cap(1).toInt();
    return  size;
}

void Server::sendRequestStatus(const char* status)
{
    DEBUG("answer : {}", status);
    for (QTcpSocket* socket : sockets) {
        DEBUG("answer : {}", status);
        socket->write(status);
    }
}
void Server::connectToDB()
{
    std::cout << "connectToDB"<<std::endl;
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
    Database.close();
    return 0;
}

void Server::receiveUsersRequest()
{

}

RequestInfo* Server::sendRequestInfo()
{
    return nullptr;
}

UserInfo* Server::sendUsersInfo()
{
    return nullptr;
}
