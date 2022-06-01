#pragma once

#include <QObject>
#include <QProcess>
#include <QThread>

#include "terminalparser.h"
#include "types/daemon.h"
#include "types/tobject.h"

class Terminal : public TObject
{
    Q_OBJECT
public:
    explicit Terminal(QObject *parent = nullptr);
    void InitConfig(Configuration);
    void Validate();

signals:
    void timeout(QString);
    void gotError(QString);
    void succsess();

public slots:
    void Pay(double sum, OperationTypes);
    void CheckStatus();
    void StartTimeout();
    void Cancel();

private:
    int m_payment;
    int m_timeOut;
    bool m_isActive = false;

private:
    QString m_bash = "loadparm.exe";
    QProcess *m_procces = new QProcess(this);

private:
    TerminalParser m_parser;
};
