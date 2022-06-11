#ifndef RESPONCE_H
#define RESPONCE_H

#include <iostream>


#include <QByteArray>

#include <charconv>

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
