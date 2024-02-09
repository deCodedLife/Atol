#pragma once

#include <QObject>
#include <QDir>
#include <QDateTime>

#include "types/tobject.h"

enum PaymentResults
{
    PAYMENT_NOTFOUND,
    PAYMENT_ERROR,
    PAYMENT_DONE,
};

struct TPayment
{
    int TargetSum;
    QTime After;
    PaymentResults Result;
};

#define TYPE_COMMAND "Command"
#define TYPE_RESULT "Result"

enum TLineTypes
{
    LINE_NONE,
    LINE_COMMAND,
    LINE_RESULT,
    LINE_BIOMETRY
};

struct TLine
{
    QString Date;
    QString Time;

    QString Sender;
    QString Action;

    QString Value;
    TLineTypes type;

    static TLine fromString(QString);
};

class TerminalParser : public TObject
{
    Q_OBJECT
public:
    explicit TerminalParser(QObject *parent = nullptr);
    void SetTarget(TPayment);
    void SetDirectory(QString);
    TPayment Parse();

private:
    bool isValid(QString);
    bool isTarget(TLine);

    void changeResult(TLine, TPayment&);

private:
    QString m_terminalFolder;
    QString m_lastLog;

    TPayment m_currentPayment;
    QStringList m_actions;
};
