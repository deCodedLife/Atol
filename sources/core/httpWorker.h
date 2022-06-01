#pragma once

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>

#include "timer.h"
#include "network.h"
#include "types/logs.h"

class HttpWorker : public QObject
{
    Q_OBJECT
signals:
    void dataRecived(QString url, QJsonObject);

public:
    explicit HttpWorker(QObject *parent = nullptr);
    void SetURL(QString, QList<QString> args);
    void Stop();

private slots:
    void MakeRequest();
    void RequestRecived(QString url, QString data);

private:
    Timer m_timer;
    QThread m_timerThread;

    Network net;

    bool m_isWorking;
    QString m_requestURL;
};

