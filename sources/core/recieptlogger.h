#pragma once

#include "types/tobject.h"

#define LOG_FOLDER "reciepts"

class RecieptLogger : public TObject
{
    Q_OBJECT
public:
    RecieptLogger(QObject *parent = nullptr);
    static QString getLogPath();

public slots:
    void HandleReciept(QJsonObject);

private:
    QJsonObject prepareTask(Task);

};

