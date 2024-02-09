#pragma once

#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#define JWTs "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpZCI6IjIiLCJpcCI6IjkyLjI1NS4yNDguNzkiLCJlbWFpbCI6InRlc3RAbWFpbC5ydSIsInJvbGVfaWQiOiIxIn0.gVAQTvbPADFBTpf7WGboN3ojRUoS4qiRd0-Q8x68EmM"

class Request
{
public:
    Request();
    QJsonObject toJson();

    void ChangeJWT(QString);
    void SetService(QString);
    void SetCommand(QString);
    void SetData(QVariant);

    static Request GetJWT(QString username, QString password);
    static Request GetPayments(QString cashbox_id, QString jwt = JWTs);
    static Request GetOperations(QString cashbox_id, QString jwt = JWTs);

    static Request ConfirmTransaction(int sale_id );
    static Request CancelTransaction(int sale_id, QString status, QString description );

    static Request StatusChangeMultiple(QList<int> sales, QString status, QString description, QString jwt = JWTs);

    static Request UpdateRecieptCode(int sale_id, QString code, QString hash = "", QString jwt = JWTs);

private:

    QString jwt = JWTs;
    QString service;
    QString command;
    QVariant data;
};

