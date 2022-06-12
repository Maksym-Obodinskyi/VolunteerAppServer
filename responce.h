#ifndef RESPONCE_H
#define RESPONCE_H

#include "RequestInfo.h"
#include "UserInfo.h"

#include <QByteArray>
#include <QString>
#include <QVector>

struct Responce {
    char type;
    int err;

    Responce(char _type = '|');

    QByteArray addItem(QString itemsField);
    QByteArray addItem(int itemsField);

    virtual QByteArray serialize();
    virtual void deserialize(QByteArray arr);
};


struct LogInResponce : Responce {
    LogInResponce();
    QByteArray serialize() override;
    void deserialize(QByteArray arr) override;
    UserInfo userInfo;
};

struct LogOutResponce : Responce {
    LogOutResponce();
};

struct NewUserResponce : Responce {
    NewUserResponce();
};

struct UpdateProfileResponce : Responce {
    UpdateProfileResponce();
};

struct AddRequestResponce : Responce {
    AddRequestResponce();
};

/////////////////////////////////////////////////////////////
/// \brief The GetRequestResponce struct
/////////////////////////////////////////////////////////////

struct GetRequestResponce : Responce {
    int err;
    QVector<RequestInfo> requestsList;
    GetRequestResponce();
    QByteArray serialize() override;
    void deserialize(QByteArray arr) override;
};
/////////////////////////////////////////////////////////////
/// \TODO: Finish GetResponce serialize
/////////////////////////////////////////////////////////////

struct UpdateRequestResponce : Responce {
    UpdateRequestResponce();
};

struct RemoveRequestResponce : Responce {
    RemoveRequestResponce();
};

#endif // RESPONCE_H
