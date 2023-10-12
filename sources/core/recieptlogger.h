#pragma once

#include <QDateTime>
#include "types/tobject.h"

#define LOG_FOLDER "receipts"

class RecieptLogger : public TObject
{
    Q_OBJECT
public:
    RecieptLogger(QObject *parent = nullptr);
    static QString getLogPath();

public slots:
    void HandleReciept(QJsonObject);

private slots:
    void init();

private:
    QJsonObject prepareTask(Task);

};

