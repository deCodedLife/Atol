#pragma once

#include <iostream>

#include <QThread>
#include <QObject>
#include <QIODevice>

#include <QTcpServer>
#include <QTcpSocket>

#include <QJsonObject>
#include <QJsonDocument>

#define PORT 7084

class HttpServer : public QObject
{
    Q_OBJECT
signals:
    void recivedRequest(QJsonObject);

public:
    explicit HttpServer(QObject *parent = nullptr);
    ~HttpServer();
    void configure(QThread &);

    QTcpSocket *socket;

public slots:
    void newConnection();
    void closeClient();
    void txRx();

private:
    void Run();
    QJsonObject ParseJson(QString);
    QTcpServer *server;
};
