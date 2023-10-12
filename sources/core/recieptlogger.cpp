#include "recieptlogger.h"

RecieptLogger::RecieptLogger(QObject *parent) : TObject(parent)
{
    logList = QMap<GLOBAL_ERRORS, Log> {
        {OPEN_FILE_ERROR, Log{"RecieptLogger", "ERROR", "Can't write reciept", ""}}
    };

    connect(this, &RecieptLogger::configsUpdated, this, &RecieptLogger::init);
}

void RecieptLogger::HandleReciept(QJsonObject task)
{
    QString fileName = getLogPath();
    QString taskName = task["uuid"].toString().toUtf8().toHex();

    fileName.append('/');
    fileName.append(taskName).append(".txt");

    if (QFile::exists(fileName))
    {
        QFile::remove(fileName);
    }

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);

    if ( file.isOpen() == false )
    {
        Log error = logList[OPEN_FILE_ERROR];
        error.details = file.errorString();

        Logger::WriteToFile(error);
        return;
    }

    QTextStream output(&file);
    output << QJsonDocument(task).toJson();

    file.close();
}

void RecieptLogger::init()
{
    QString logPath = QDir::currentPath();
    logPath.append('/');
    logPath.append(LOG_FOLDER);

    if ( QDir().exists(logPath) == false )
    {
        QDir().mkdir(logPath);
    }
}

QString RecieptLogger::getLogPath()
{
//    return QDir::currentPath().append('/').append(LOG_FOLDER);
    return LOG_FOLDER;
}
