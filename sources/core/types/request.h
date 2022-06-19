#pragma once

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
    void SetData(QJsonObject);

    static Request GetJWT(QString username, QString password);
    static Request GetPayments(QString cashbox_id, QString jwt = JWTs);
    static Request GetOperations(QString cashbox_id, QString jwt = JWTs);
    static Request StatusChange(int sale_id, QString status, QString description, QString jwt = JWTs);
    static Request UpdateRecieptCode(int sale_id, QString code, QString jwt = JWTs);

private:

    QString jwt = JWTs;
    QString service;
    QString command;
    QJsonObject data;
};

