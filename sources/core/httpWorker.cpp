#include "httpWorker.h"

HttpWorker::HttpWorker(QObject *parent) : QObject(parent)
{
    m_timer.SetTime(100);

    connect(&m_timer, &Timer::timeout, this, &HttpWorker::MakeRequest);
    connect(&net, &Network::externRecived, this, &HttpWorker::RequestRecived);

    m_timer.configure(m_timerThread);
    m_timer.moveToThread(&m_timerThread);
    m_timerThread.start();
}

void HttpWorker::SetURL(QString url, QList<QString> args)
{
    m_requestURL = url;
    m_isWorking = true;

    while ( args.isEmpty() == false )
    {
        m_requestURL += '/' + args.takeFirst();
    }

    Log newLog;
    newLog.type = "INFO";
    newLog.sender = "NetWorker";
    newLog.data = "New URL setted";
    newLog.details = m_requestURL;

    Logger::WriteToFile(newLog);
}

void HttpWorker::Stop()
{
    m_isWorking = false;
    net.Abort();
}

void HttpWorker::MakeRequest()
{
    if (m_isWorking == false)
    {
        return;
    }

    net.Get(m_requestURL);
}

void HttpWorker::RequestRecived(QString url, QString data)
{
    QJsonObject reply = QJsonDocument::fromJson(data.toUtf8()).object();

    Log newLog;
    newLog.sender = "NETWORKER";
    newLog.type = "INFO";
    newLog.data = "Atol recived";
    newLog.details = data;

    Logger::WriteToFile(newLog);

    emit dataRecived(url, reply);
}


