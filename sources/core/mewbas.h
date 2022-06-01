#pragma once

#include <QObject>
#include <QJsonObject>
#include <QJsonParseError>

#include "network.h"
#include "types/tobject.h"
#include "types/daemon.h"

class Mewbas : public TObject
{
    Q_OBJECT
public:
    explicit Mewbas(QObject *parent = nullptr);
    Daemon RequestDaemon;

signals:
    void newTask(QJsonObject);

public slots:
    void SendRequest(Request);
    void ChangeStatus(Task);
    void NextTask();

private slots:
    void updatePayments();
    void updateOperations();

    void gotData(QString data);
    bool exists(QJsonObject);

private:
    Network m_net;

    QJsonObject m_currentTask;
    QList<Task> m_queue = QList<Task>();
};
