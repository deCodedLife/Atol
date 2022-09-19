#pragma once

#include <QObject>

#include "httpWorker.h"
#include "network.h"
#include "terminal.h"

#include "types/daemon.h"
#include "types/tobject.h"

#define AUTO_CONFIRM false
#define CASHBOX_STATUS "Печать чека"
#define TERMINAL_STATUS "Оплата терминалом"

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
    Q_PROPERTY(QString cashboxStatus READ QMLcashboxStatus)
    Q_PROPERTY(QString terminalStatus READ QMLterminalStatus)

public:
    explicit Server(QObject *parent = nullptr);

    Daemon TimeoutDaemon;
    Daemon EpayTimeout;

    Q_INVOKABLE QString qmlGetStatus();
    Q_INVOKABLE void qmlCancelOperation();
    Q_INVOKABLE void qmlRestoreServer();
    Q_INVOKABLE void qmlReturnEpay(double sum);

signals:
    void updateStatus(Task);
    void updateRecieptCode(Task, QString);
    void statusChanged(QString);

    void notify(QString title, QString message);
    void taskEnded();
    void skipped();
    void block();
    void restore();

public slots:
    void GotTask(QJsonObject);
    void GotResponse(QString);
    void OperationFailed(QString description);
    void HttpWorkerRecived(QString from, QJsonObject);
    void AtolRecived(QJsonObject);

private slots:
    void printCashcheck();
    void cancelPrint();
    void cancelOperation();
    void handleRecieptCode(QString);

    QString QMLcashboxStatus();
    QString QMLterminalStatus();

private:
    Task currentTask;
    bool m_isBlocked;

private:
    Network net;
    Terminal eCash;

private:
    QString currentStatus;
    SERVER_STATES state;

private:
#if (AUTO_CONFIRM)
    HttpWorker worker;
#endif
};
