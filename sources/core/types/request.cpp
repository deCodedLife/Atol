#include "request.h"

Request::Request() {}

QJsonObject Request::toJson()
{
    QJsonObject request;
    request["service"] = service;
    request["command"] = command;
    request["data"] = QJsonValue::fromVariant( data );
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

#ifdef QT_DEBUG
    mewbasRequest.SetCommand("getPayments_beta");
#else
    mewbasRequest.SetCommand("getPayments_beta");
#endif

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

Request Request::StatusChange(int sale_id, QString status, QString description, QString hash, QString jwt)
{
    QJsonObject requestData;

    requestData["status"] = status;
    requestData["description"] = description;

    if ( hash.isEmpty() == false )
    {
        requestData["hash"] = hash;
        requestData["by_hash"] = true;
    }
    else
    {
        requestData["id"] = sale_id;
    }

    Request mewbasRequest;
    mewbasRequest.SetService("sales");
    mewbasRequest.SetCommand("change");
    mewbasRequest.SetData(requestData);
    mewbasRequest.ChangeJWT(jwt);

    return mewbasRequest;
}

Request Request::StatusChangeMultiple(QList<int> sales, QString status, QString description, QString jwt)
{
    QJsonObject requestData;
    QJsonArray requestsSales;

    foreach( int sale_id, sales )
    {
        QJsonObject data;

        data["id"] = sale_id;
        data["status"] = status;
        data["description"] = description;

        requestsSales.append(data);
    }

    requestData["sales"] = requestsSales;

    Request mewbasRequest;
    mewbasRequest.SetService("sales");
    mewbasRequest.SetCommand("changeMultiple");
    mewbasRequest.SetData(requestData);
    mewbasRequest.ChangeJWT(jwt);

    qDebug() << QJsonDocument( mewbasRequest.toJson() ).toJson();

    return mewbasRequest;
}

Request Request::UpdateRecieptCode(int sale_id, QString code, QString hash, QString jwt)
{
    QJsonObject requestData;
    requestData["id"] = sale_id;
    requestData["code_return"] = code;

    Request mewbasRequest;
    mewbasRequest.SetService("sales");
    mewbasRequest.SetCommand("change");

    if ( hash.isEmpty() == false )
    {
        requestData["hash"] = hash;
        requestData["by_hash"] = true;
    }

    mewbasRequest.SetData(requestData);
    mewbasRequest.ChangeJWT(jwt);

    return mewbasRequest;
}
