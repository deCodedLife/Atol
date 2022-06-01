#pragma once

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QObject>
#include <QDateTime>

#define LOG_FILE "logs.txt"
#define SEPARATOR "\t"
#define ENDLINE "\r\n"

struct Log
{
    QString type;
    QString sender;
    QString data;
    QString details;
};

class Logger : public QObject
{
    Q_OBJECT
public:
    Logger();

public slots:
    static void WriteToFile(Log);
};
