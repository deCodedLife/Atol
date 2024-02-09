#include "request.h"

Request::Request() {}

QJsonObject Request::toJson()
{
    QJsonObject request;
    request["command"] = command;
    request["object"] = service;
    request["data"] = QJsonValue::fromVariant( data );

    return request;
}

void Request::ChangeJWT(QString key)
{
    if ( jwt == key )
    {
        return;
    }

    jwt = key;
}

void Request::SetService(QString s)
{
    if ( service == s )
    {
        return;
    }

    service = s;
}

void Request::SetCommand(QString c)
{
    if ( command == c )
    {
        return;
    }

    command = c;
}

void Request::SetData(QVariant d)
{
    data = d;
}

Request Request::GetJWT(QString username, QString password)
{
    QJsonObject requestData;
    requestData["email"] = username;
    requestData["password"] = password;

    Request mewbasRequest;

    mewbasRequest.service = "employees";
    mewbasRequest.command = "sign-in";
    mewbasRequest.data = requestData;

    return mewbasRequest;
}

Request Request::GetPayments(QString cashbox_id, QString jwt)
{
    QJsonObject requestData;
    requestData["cashbox_id"] = cashbox_id;

    Request mewbasRequest;
    mewbasRequest.SetService("atol");
    mewbasRequest.SetCommand("get-transactions");

    mewbasRequest.SetData(requestData);
    //    mewbasRequest.ChangeJWT(jwt);

    return mewbasRequest;
}

Request Request::GetOperations(QString cashbox_id, QString jwt)
{
    QJsonObject requestData;
    requestData["cashbox_id"] = cashbox_id;

    Request mewbasRequest;
    mewbasRequest.SetService("atol");
    mewbasRequest.SetCommand("get-operations");
    mewbasRequest.SetData(requestData);

    return mewbasRequest;
}

Request Request::ConfirmTransaction(int sale_id )
{
    QJsonObject requestData;
    requestData["sale_id"] = sale_id;
    requestData["status"] = "done";

    Request mewbasRequest;
    mewbasRequest.SetService("atol");
    mewbasRequest.SetCommand("confirm-transaction");
    mewbasRequest.SetData(requestData);

    return mewbasRequest;
}

Request Request::CancelTransaction(int sale_id, QString status, QString description )
{
    QJsonObject requestData;

    requestData["sale_id"] = sale_id;
    requestData["status"] = status;
    requestData["description"] = description;

    Request mewbasRequest;
    mewbasRequest.SetService("atol");
    mewbasRequest.SetCommand("decline-transaction");
    mewbasRequest.SetData(requestData);

    return mewbasRequest;
}

Request Request::StatusChangeMultiple(QList<int> sales, QString status, QString description, QString jwt)
{
    QJsonObject requestData;
    QJsonArray requestsSales;

    requestData["sale_id"] = sales.first();
    requestData["status"] = status;
    requestData["description"] = description;

    Request mewbasRequest;
    mewbasRequest.SetService("atol");
    mewbasRequest.SetCommand("confirm-transaction");
    mewbasRequest.SetData(requestData);
    //    mewbasRequest.ChangeJWT(jwt);

    qDebug() << QJsonDocument( mewbasRequest.toJson() ).toJson();

    return mewbasRequest;
}

Request Request::UpdateRecieptCode(int sale_id, QString code, QString hash, QString jwt)
{
    QJsonObject requestData;
    requestData["id"] = sale_id;
    requestData["terminal_code"] = code;

    Request mewbasRequest;
    mewbasRequest.SetService("salesList");
    mewbasRequest.SetCommand("update");

    if ( hash.isEmpty() == false && hash.isNull() == false && hash != "null" )
    {
        requestData["hash"] = hash;
        requestData["by_hash"] = true;
    }

    mewbasRequest.SetData(requestData);
//    mewbasRequest.ChangeJWT(jwt);

    return mewbasRequest;
}
