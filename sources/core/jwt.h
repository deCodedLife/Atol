#pragma once

#include <QObject>

#include "types/configs.h"
#include "network.h"
#include "types/request.h"
#include "types/logs.h"

enum JWT_Errors
{
    EMPTY_RESPONCE,
    WRONG_DATA,
    WRONG_SERVER
};

class JWT : public QObject
{
    Q_OBJECT
signals:
    void updateJWT(QString);

public:
    explicit JWT(QObject *parent = nullptr);
    void Init(Configs *cfg);

private:
    QMap<JWT_Errors, Log> errorsList;

    Network net;
    Configs *configs;
    Configuration currentConfiguration;

    void sendJWTRequest();

public slots:
    void ReciveJWT(QString);
    void SetConfiguration(Configuration c);

};

