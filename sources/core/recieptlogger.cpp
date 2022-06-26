#include "recieptlogger.h"

RecieptLogger::RecieptLogger(QObject *parent) : TObject(parent)
{
    logList = QMap<GLOBAL_ERRORS, Log> {
        {OPEN_FILE_ERROR, Log{"RecieptLogger", "ERROR", "Can't write reciept", ""}}
    };

    QString logPath = QDir::currentPath();
    logPath.append('/');
    logPath.append(LOG_FOLDER);

    if ( QDir().exists(logPath) == false )
    {
        QDir().mkdir(logPath);
    }
}

void RecieptLogger::HandleReciept(QJsonObject task)
{
    QString fileName = getLogPath();
    QString taskName = task["uuid"].toString().toUtf8().toHex();

    fileName.append('/');
    fileName.append(fileName.append(".json"));

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);

    if ( file.isOpen() == false )
    {
        Log error = logList[OPEN_FILE_ERROR];
        error.details = file.errorString();

        Logger::WriteToFile(error);
        return;
    }

    QDataStream output(&file);
    output << task;

    file.close();
}

QString RecieptLogger::getLogPath()
{
    return QDir::currentPath().append('/').append(LOG_FOLDER);
}
