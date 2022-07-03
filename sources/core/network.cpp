#include "network.h"

Network::Network(QObject *parent) : QObject(parent), isFinished(true)
{
    QObject::connect(netMgr, &QNetworkAccessManager::finished, this, &Network::gotData);
}

bool Network::GetStatus()
{
    return isFinished;
}

void Network::Get(QString uri)
{
    isFinished = false;
    netRequest.setUrl(QUrl(uri));
    netMgr->get(netRequest);
}

void Network::Delete(QString uri)
{
    isFinished = false;
    netRequest.setUrl(QUrl(uri));
    netMgr->deleteResource(netRequest);
}

void Network::Post(QString uri, QJsonObject data)
{
    isFinished = false;
    netRequest.setUrl(QUrl(uri));

    QJsonDocument jsonObject(data);
    QByteArray jsonData = jsonObject.toJson();

    netRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    netRequest.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(jsonData.size()));

    netMgr->post(netRequest, jsonData);
}

void Network::GetSync(QString uri)
{
    isFinished = false;
    QEventLoop *loop = new QEventLoop();

    QNetworkRequest *request = new QNetworkRequest();
    QNetworkAccessManager *mgr = new QNetworkAccessManager();

    request->setUrl(QUrl(uri));
    mgr->get(netRequest);

    connect(mgr, SIGNAL(finished(QNetworkReply*)), loop, SLOT(quit()));
    connect(mgr, &QNetworkAccessManager::finished, this, &Network::gotData);

    loop->exec();
}

void Network::DeleteSync(QString uri)
{
    isFinished = false;
    QEventLoop *loop = new QEventLoop();

    QNetworkRequest *request = new QNetworkRequest();
    QNetworkAccessManager *mgr = new QNetworkAccessManager();

    request->setUrl(QUrl(uri));
    mgr->deleteResource(netRequest);

    connect(mgr, SIGNAL(finished(QNetworkReply*)), loop, SLOT(quit()));
    connect(mgr, &QNetworkAccessManager::finished, this, &Network::gotData);

    loop->exec();
}

void Network::PostSync(QString uri, QJsonObject data)
{
    isFinished = false;
    QEventLoop *loop = new QEventLoop();

    QNetworkRequest *request = new QNetworkRequest();
    QNetworkAccessManager *mgr = new QNetworkAccessManager();
    QNetworkReply *reply;

    QJsonDocument jsonObject(data);
    QByteArray jsonData = jsonObject.toJson();

    request->setUrl(QUrl(uri));
    netRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    netRequest.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(jsonData.size()));

    reply = mgr->post(netRequest, jsonData);
    connect(mgr, SIGNAL(finished(QNetworkReply*)), loop, SLOT(quit()));

    loop->exec();

    gotData(reply);
}

void Network::Abort()
{
    shouldAbort = true;
}

void Network::gotData(QNetworkReply *netReply)
{
    isFinished = true;
    QString data = netReply->readAll();

    if (shouldAbort)
    {
        shouldAbort = false;
        return;
    }

    if ( data.trimmed() == "" )
    {
        emit timeout("[ERROR] [NETWORK] No network connection");
        return;
    }
    if ( netReply->error() == QNetworkReply::TimeoutError )
    {
        emit timeout("[ERROR] [NETWORK] Can't reach address: " + netReply->url().toString());
        return;
    }

    emit externRecived(netReply->url().toString(), data);
    emit dataRecived(data);
}
