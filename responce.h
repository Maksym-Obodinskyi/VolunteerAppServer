#ifndef RESPONCE_H
#define RESPONCE_H

#include <QByteArray>

#include <charconv>

struct Responce {
    char type;
    int err;

    Responce(char _type = '|') : type(_type) {}

    virtual QByteArray serialize()
    {
        QByteArray ret;
        ret += type;
        ret += ':';
        char arr[5];
        auto [ptr, ec] = std::to_chars(arr, arr + 5, err);
        if (ec == std::errc()) {
            arr[ptr - arr] = '\0';
            ret += arr;
        } else {
            ret += '9';
        }
        return ret;
    }

    virtual void deserialize(QByteArray arr)
    {
        if (arr.size() > 1) {
            int result;
            const auto[_, ec] = std::from_chars(arr.constData() + 2, arr.constData() + arr.size(), result);

            if (ec == std::errc()) {
                err = result;
            } else {
                err = 1000;
            }
        } else {
            err = 1001;
        }
    }
};


struct LogInResponce : Responce {
    LogInResponce() : Responce('l') {}
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
    GetRequestResponce() : Responce('g') {}
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
