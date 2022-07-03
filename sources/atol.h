#pragma once

#include <QObject>

#include "core/mewbas.h"
#include "core/server.h"
#include "core/reciepts.h"
#include "core/httpserver.h"
#include "core/jwt.h"

#include "core/types/configs.h"

class Atol : public QObject
{
    Q_OBJECT
public:
    explicit Atol(QObject *parent = nullptr);
    void ChangeConfiguration(Configs *cfg);

    Mewbas api;
    Server core;
    Reciepts reciepts;

public slots:
    void Ready();

private:
    Timer m_tick;
    QThread m_timerThread;

    JWT jwt;

    Configs *cfg;
    Configuration configuration;

    QThread serverThread;
    HttpServer httpSrv;
};
