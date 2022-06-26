#include "reciepts.h"

void Reciepts::remove(int id)
{
    QJsonObject taskJson = m_reciepts.at(id);
    taskJson.remove("callbacks");

    QJsonObject request = taskJson["request"].toObject();
    request["type"] = "sellReturn";

    taskJson["request"] = request;
    taskJson["uuid"] = taskJson["uuid"].toString().append("HandReturn");

    net.Post(m_configuration.serverAddr + "/api/v2/requests", taskJson);
    taskJson.insert("active", false);

    m_logger.HandleReciept(taskJson);
    m_reciepts.removeAt(id);

    emit recieptsChanged(m_reciepts);
    emit removed(id);
}

Reciepts::Reciepts(QObject *parent) : TObject(parent)
{
    logList = QMap<GLOBAL_ERRORS, Log> {};
    LoadReciepts();
}

void Reciepts::HandleReciept(Task t)
{
    m_logger.HandleReciept(t.task);
    m_reciepts.append(t.task);

    emit recieptsChanged(m_reciepts);
}

void Reciepts::LoadReciepts()
{
    m_reciepts.clear();

    QString logsFolder = RecieptLogger::getLogPath();
    QStringList logsFiles = QDir(logsFolder).entryList(QStringList() << "*.json", QDir::Files);

    foreach( QString fileName, logsFiles )
    {
        QFile logFile(fileName);
        logFile.open(QIODevice::ReadOnly);

        if ( logFile.isOpen() == false )
        {
            continue;
        }

        QString fileContent = logFile.readAll();
        QJsonObject reciept = QJsonDocument::fromJson(fileContent.toUtf8()).object();

        m_reciepts.append(reciept);
        logFile.close();
    }

    emit recieptsChanged(m_reciepts);
}

QList<QJsonObject> Reciepts::QMLReciepts()
{
    return m_reciepts;
}




