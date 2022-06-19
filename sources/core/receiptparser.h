#pragma once

#include <QFile>
#include <QObject>

#include "types/tobject.h"

#define PATTERN "RRN:"

class ReceiptParser : public TObject
{
    Q_OBJECT
signals:
    void codeDetected(QString);

public:
    ReceiptParser(QObject *parent = nullptr);

public slots:
    void StartParsing();
    void SetTarget(QString);

private:
    void parse(QString data);

private:
    QString m_target;

};

