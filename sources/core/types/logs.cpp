#include "logs.h"

Logger::Logger() {}

void Logger::WriteToFile(Log data)
{
    QFile fileInstance = QFile(LOG_FILE);
    fileInstance.open(QIODevice::WriteOnly | QIODevice::Append);

    if ( fileInstance.isOpen() == false )
    {
        qCritical() << "ERROR: Can't open log file. Reason" << fileInstance.errorString();
        //quick_exit(-2);
    }

    QTextStream out(&fileInstance);
    QString logString;

    logString.append(QDateTime::currentDateTime().time().toString());
    logString.append(SEPARATOR);

    logString.append(data.type);
    logString.append(SEPARATOR);

    logString.append(data.sender);
    logString.append(SEPARATOR);

    logString.append(data.data);

    if ( data.details.trimmed().isEmpty() == false )
    {
        logString.append(SEPARATOR);

        logString.append("Details: ");
        logString.append(SEPARATOR);

        logString.append(data.details);
    }

    logString.append(ENDLINE);

    out << logString;
    fileInstance.close();
}
