#pragma once

#include <QDateTime>
#include <QJsonObject>
#include <QDirIterator>

#include "network.h"
#include "types/tobject.h"
#include "recieptlogger.h"

class Reciepts : public TObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QJsonObject> reciepts READ QMLReciepts NOTIFY recieptsChanged)

signals:
    void recieptsChanged(QList<QJsonObject>);

public:
    Reciepts(QObject *parent = nullptr);

    Q_INVOKABLE void remove(QJsonObject);
    Q_INVOKABLE void print(QJsonObject);
    Q_INVOKABLE void qmlReturnEpay(QJsonObject);
    
    QList<QJsonObject> QMLReciepts();

public slots:
    void HandleReciept(Task t);

private:
    void loadReciepts();
    int search(QJsonObject task);
    QJsonObject clearTask(QJsonObject);

private:
    Network net;
    RecieptLogger m_logger;

    QList<QJsonObject> m_reciepts;
};

