#include "reciepts.h"

void Reciepts::remove(QJsonObject task)
{
    QJsonObject taskJson = task;
    taskJson.remove("callbacks");
    taskJson.remove("time");
    taskJson.remove("date");
    taskJson.remove("status");
    taskJson.remove("active");

    QJsonObject request = taskJson["request"].toObject();
    request["type"] = "sellReturn";

    taskJson["request"] = request;
    taskJson["uuid"] = taskJson["uuid"].toString().append("HandReturn");

    net.Post(m_configuration.serverAddr + "/api/v2/requests", taskJson);

    task.insert("active", false);

    m_logger.HandleReciept(task);
    m_reciepts.removeAt(search(task));

    emit recieptsChanged(m_reciepts);
}

void Reciepts::qmlReturnEpay(QJsonObject task)
{
    task.insert("active", false);
    m_logger.HandleReciept(task);

    m_reciepts.removeAt(search(task));
    emit recieptsChanged(m_reciepts);
}

Reciepts::Reciepts(QObject *parent) : TObject(parent)
{
    logList = QMap<GLOBAL_ERRORS, Log> {};
    loadReciepts();
}

void Reciepts::HandleReciept(Task t)
{
    QString status = t.status == TASK_SUCCSESS ? "done" : "error";

    QJsonObject recieptJson = t.task;
    recieptJson.insert("time", QDateTime().currentDateTime().time().toString("HH:mm:ss"));
    recieptJson.insert("date", QDateTime().currentDateTime().date().toString("yyyy.MM.dd"));
    recieptJson.insert("status", status);
    recieptJson.insert("active", true);

    m_logger.HandleReciept(recieptJson);
    m_reciepts.append(recieptJson);

    emit recieptsChanged(m_reciepts);
}

void Reciepts::loadReciepts()
{
    m_reciepts.clear();

    QString logsFolder = RecieptLogger::getLogPath();
    QStringList logsFiles = QDir(logsFolder).entryList(QStringList() << "*", QDir::Files);

    foreach( QString fileName, logsFiles )
    {
        QFile logFile(RecieptLogger::getLogPath().append('/').append(fileName));
        logFile.open(QIODevice::ReadOnly);

        if ( logFile.isOpen() == false )
        {
            continue;
        }

        QString fileContent = logFile.readAll();
        QJsonObject reciept = QJsonDocument::fromJson(fileContent.toUtf8()).object();

        if ( QDate::fromString(reciept["date"].toString(), "yyyy.MM.dd") != QDate::currentDate() )
        {
            continue;
        }

        if ( reciept["active"].toBool() == false )
        {
            continue;
        }

        if ( reciept["request"].toObject()["type"].toString() == "reportX" )
        {
            continue;
        }

        m_reciepts.append(reciept);
        logFile.close();
    }

    emit recieptsChanged(m_reciepts);
}

int Reciepts::search(QJsonObject task)
{
    int id = 0;

    foreach( QJsonObject i, m_reciepts )
    {
        if ( i["uuid"].toString() == task["uuid"].toString() )
        {
            return id;
        }

        id++;
    }

    return id;
}

QList<QJsonObject> Reciepts::QMLReciepts()
{
    return m_reciepts;
}
