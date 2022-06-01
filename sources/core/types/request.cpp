#include "request.h"

Request::Request() {}

QJsonObject Request::toJson()
{
    QJsonObject request;
    request["service"] = service;
    request["command"] = command;
    request["data"] = data;
    request["jwt"] = jwt;

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

void Request::SetData(QJsonObject d)
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

#ifdef QT_DEBUG
    requestData["test"] = true;
#endif

    Request mewbasRequest;
    mewbasRequest.SetService("atol");
    mewbasRequest.SetCommand("getPayments");
    mewbasRequest.SetData(requestData);
    mewbasRequest.ChangeJWT(jwt);

    return mewbasRequest;
}

Request Request::GetOperations(QString cashbox_id, QString jwt)
{
    QJsonObject requestData;
    requestData["cashbox_id"] = cashbox_id;

    Request mewbasRequest;
    mewbasRequest.SetService("atol");
    mewbasRequest.SetCommand("getOperations");
    mewbasRequest.SetData(requestData);
    mewbasRequest.ChangeJWT(jwt);

    return mewbasRequest;
}

Request Request::StatusChange(int sale_id, QString status, QString description, QString jwt)
{
    QJsonObject requestData;
    requestData["id"] = sale_id;
    requestData["status"] = status;
    requestData["description"] = description;

    Request mewbasRequest;
    mewbasRequest.SetService("sales");
    mewbasRequest.SetCommand("change");
    mewbasRequest.SetData(requestData);
    mewbasRequest.ChangeJWT(jwt);

    return mewbasRequest;
}
