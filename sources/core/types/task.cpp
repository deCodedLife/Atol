#include "task.h"

Product Product::parse(QJsonValue p)
{
    Product product;
    QJsonObject productJson = p.toObject();

    product.amount = productJson["amount"].toDouble();
    product.name = productJson["name"].toString();
    product.paymentObject = productJson["paymentObject"].toString();
    product.piece = productJson["piece"].toBool();
    product.price = productJson["price"].toDouble();
    product.quantity = productJson["quantity"].toInt();
    product.tax = productJson["tax"].toObject();
    product.type = productJson["type"].toString();

    return product;
}


Task Task::parse(QJsonObject json)
{
    Task recievedTask;
    QJsonObject callback;
    QJsonObject mewbassTask = json["data"].toObject();
    QJsonObject mewbassRequest = mewbassTask["request"].toObject();

    // Is operation or task
    recievedTask.isValid = true;
    recievedTask.type = OPERATIONS_None;
    recievedTask.status = TASK_NONE;
    recievedTask.description = "";
    recievedTask.ePaySum = 0;
    recievedTask.ePay = false;
    recievedTask.isOperation = false;
    recievedTask.hash = mewbassTask["hash"].toString();
    recievedTask.uuid = mewbassRequest["uuid"].toString();
    callback["resultUrl"] = CALLBACK_URL;

    if ( mewbassTask["sales"].toArray().isEmpty() )
    {
        // OPERATION
        QJsonObject customTask;
        QJsonObject customRequest;

        customRequest["type"] = mewbassRequest["type"];
        customRequest["operator"] = mewbassRequest["operator"];

        customTask["uuid"] = recievedTask.uuid;
        customTask["callbacks"] = callback;
        customTask["request"] = customRequest;

        recievedTask.isOperation = true;
        recievedTask.task = customTask;

        return recievedTask;
    }

    // TASK
    double productsSum {0};
    recievedTask.saleID = mewbassTask["sale_id"].toString().toInt();

    foreach( QJsonValue sale, mewbassTask["sales"].toArray() )
    {
        recievedTask.sales.append( sale.toInt() );
    }

    recievedTask.task = mewbassRequest;
    recievedTask.task["callbacks"] = callback;

    QJsonObject secondRequest = mewbassRequest["request"].toObject();
    QJsonArray payments = secondRequest["payments"].toArray();

    if ( secondRequest["items"].toArray().count() == 1 )
    {
        recievedTask.isValid = false;
    }

    foreach ( QJsonValue itemJson, secondRequest["items"].toArray() )
    {
        Product item = Product::parse(itemJson);
        productsSum += item.amount;
    }

    if ( secondRequest["type"].toString() == "sell" )
    {
        recievedTask.type = OPERATIONS_Sale;
    }

    else if ( mewbassTask["sale_type"].toString() == "return" )
    {
        recievedTask.type = OPERATIONS_Return;
        recievedTask.returnCode = mewbassTask["code_return"].toString();
    }

    else if ( mewbassTask["sale_type"].toString() == "cancel" )
    {
        recievedTask.type = OPERATIONS_Cancel;
    }

    double paymentSum = {0};

    foreach ( QJsonValue paymentValue, payments )
    {
        QJsonObject payment = paymentValue.toObject();
        paymentSum += payment["sum"].toDouble();

        if ( payment["type"].toString() == "1" )
        {
            recievedTask.ePay = true;
            recievedTask.ePaySum += payment["sum"].toDouble();
        }
    }

    paymentSum = std::ceil( paymentSum * 100 ) / 100;
    productsSum = std::ceil( productsSum * 100 ) / 100;

    if ( paymentSum > productsSum )
    {
        recievedTask.isValid = false;
    }

    return recievedTask;
}
