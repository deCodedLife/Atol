#pragma once

#include <QJsonObject>

#include "network.h"
#include "types/tobject.h"
#include "recieptlogger.h"

class Reciepts : public TObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QJsonObject> reciepts READ QMLReciepts NOTIFY recieptsChanged)

signals:
    void recieptsChanged(QList<QJsonObject>);
    void removed(int);

public:
    Reciepts(QObject *parent = nullptr);

    Q_INVOKABLE void remove(int);
    QList<QJsonObject> QMLReciepts();

public slots:
    void HandleReciept(Task t);

private:
    void LoadReciepts();

private:
    Network net;
    RecieptLogger m_logger;

    QList<QJsonObject> m_reciepts;
};

