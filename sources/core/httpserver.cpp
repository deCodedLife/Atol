#include "httpserver.h"

HttpServer::HttpServer(QObject *parent) : QObject(parent) {}

HttpServer::~HttpServer()
{
    server->close();
    server->disconnect();
    server->deleteLater();
}

void HttpServer::configure(QThread &thread)
{
    QObject::connect(&thread, &QThread::started, this, &HttpServer::Run);
}

QJsonObject HttpServer::ParseJson(QString htmlData)
{
    QJsonObject output;
    QList<QString> firstPart = htmlData.split("{");

    if (firstPart.length() < 2)
    {
        return output;
    }

    QString data;

    for (int i = 1; i < firstPart.length(); i++) {
        data += "{" + firstPart[i];
    }

    output = QJsonDocument::fromJson(data.toUtf8()).object();

    return output;
}

void HttpServer::txRx()
{
    QString data;
    data = socket->readAll();

    QJsonObject jsonData = ParseJson(data);

    if ( jsonData.isEmpty() == false )
    {
        emit recivedRequest(jsonData);
    }

    socket->write("HTTP/1.1 200 OK\r\n");
    socket->write("Content-Type: application/json\r\n");
    socket->write("Connection: close\r\n");
    socket->write("Refresh: 1\r\n");
    socket->write("Pragma: no-chache\r\n");
    socket->write("\r\n");
    socket->write("{\"status\": 200}");

    socket->disconnectFromHost();
}

void HttpServer::Run()
{
    server = new QTcpServer(this);
    QObject::connect(server, &QTcpServer::newConnection, this, &HttpServer::newConnection);

    if (server->listen(QHostAddress::LocalHost, PORT) == false)
    {
        qDebug() << "[Error] can't start server";
    } else
    {
        qDebug() << "[INFO] Server started at " << PORT << " port";
    }
}

void HttpServer::newConnection()
{
    socket = server->nextPendingConnection();
    QObject::connect(socket, &QTcpSocket::readyRead, this, &HttpServer::txRx);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &HttpServer::closeClient);
}

void HttpServer::closeClient()
{
    socket->deleteLater();
}
