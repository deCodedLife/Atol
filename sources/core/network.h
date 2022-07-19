#pragma once

#include <QObject>
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonDocument>

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkInterface>

class Network : public QObject
{
    Q_OBJECT
signals:
    void dataRecived(QString);
    void externRecived(QString url, QString data);
    void timeout(QString);

public:
    explicit Network(QObject *parent = nullptr);
    bool GetStatus();

    void Get(QString uri);
    void Post(QString uri, QJsonObject data);
    void Delete(QString uri);

    void GetSync(QString uri);
    void PostSync(QString uri, QJsonObject data);
    void DeleteSync(QString uri);

    void Abort();

private slots:
    void gotData(QNetworkReply *);

private:
    QNetworkAccessManager *m_netMgr = new QNetworkAccessManager(this);
    QNetworkRequest m_netRequest;

private:
    bool isFinished;
    bool shouldAbort;
};
