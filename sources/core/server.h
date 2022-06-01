#ifndef SERVER_H
#define SERVER_H

#include <QObject>

#include "httpWorker.h"
#include "network.h"
#include "terminal.h"

#include "types/daemon.h"
#include "types/tobject.h"

enum SERVER_STATES
{
    STATE_NONE,
    TERMINAL,
    CASHBOX
};

class Server : public TObject
{
    Q_OBJECT
    Q_PROPERTY(QString status READ qmlGetStatus NOTIFY statusChanged)
public:
    explicit Server(QObject *parent = nullptr);

    Daemon TimeoutDaemon;
    Daemon EpayTimeout;

    Q_INVOKABLE QString qmlGetStatus();
    Q_INVOKABLE void qmlCancelOperation();

signals:
    void updateStatus(Task);
    void statusChanged(QString);

    void taskEnded();
    void skipped();

public slots:
    void GotTask(QJsonObject);
    void GotResponse(QString);
    void OperationFailed(QString description);
    void AtolRecived(QString from, QJsonObject);

private slots:
    void printCashcheck();
    void cancelOperation();

private:
    Network net;
    Task currentTask;

    Terminal eCash;
    // HttpWorker worker;

    QString currentStatus;
    SERVER_STATES state;
};

#endif // SERVER_H
