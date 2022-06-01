#pragma once

#include <QDir>
#include <QFile>
#include <QObject>
#include <QSettings>

#include <QJsonObject>
#include <QJsonDocument>

#include "logs.h"

#define JWT_USERNAME "test@mail.ru"
#define JWT_PASSWORD "1234"
#define CASHBOX_ID "1000"
#define MEWBASS_ADDRESS "https://api.mewbas.com"
#define SERVER_ADDRESS "http://localhost:16732"
#define SERVER_ID "1001"
#define TERMINAL_DIR "C:/sc552"
#define AUTOSTART true

#define PARAM_CASHBOX_ID "cashboxID"
#define PARAM_MEWBASS_ADDRESS "mewbasAddr"
#define PARAM_SERVER_ADDRESS "serverAddr"
#define PARAM_SERVER_ID "serverID"
#define PARAM_TERMINAL_DIR "terminalDir"
#define PARAM_JWT "jwt"
#define PARAM_JWT_USERNAME "jwtUsername"
#define PARAM_JWT_PASSWORD "jwtPassword"
#define PARAM_AUTOSTART "autostart"
#define PARAM_INSTALL_PATH "installPath"
#define PARAM_APP_EXECUTABLE "appExecutable"

#define REGESTRY_PATH "HKEY_CURRENT_USER\\SOFTWARE\\AtolServer"
#define REGESTRY_AUTOSTART "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"

class Configuration
{
public:
    Configuration() {};

    static Configuration fromJson(QJsonObject);
    QJsonObject toJson();

    QString serverID;
    QString cashboxID;
    QString mewbasAddr;
    QString serverAddr;
    QString terminalDir;
    QString localAddr;
    QString jwt;
    QString jwtUsername;
    QString jwtPassword;
    QString installPath;
    QString appExecutable;
    bool autostart;
};

enum Errors
{
    OPENING_FILE_ERROR,
    WRONG_CONFIGURATION,
    FILE_IS_EMPTY
};

class Configs : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJsonObject data READ QMLGetConfigs WRITE Update NOTIFY configsLoaded)
signals:
    void configsLoaded(Configuration c);

public:
    Configs ();
    void Read();
    void Update(QJsonObject newConfiguration);
    void Init();

    QString QMLGetAppPath();
    QJsonObject QMLGetConfigs();

    Q_INVOKABLE void switchAutostart(QJsonObject c);

private:
    void write(Configuration);
    bool isValid(QJsonObject newConfigurations);

    QString regestryPath;
    QMap<Errors, Log> errorsList;
    QSettings configs;

    Configuration lastUpdated;
};
