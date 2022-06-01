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
    recievedTask.uuid = mewbassRequest["uuid"].toString();
    callback["resultUrl"] = CALLBACK_URL;

    if ( mewbassTask["sale_id"].isNull()  )
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
    recievedTask.task = mewbassRequest;
    recievedTask.task["callbacks"] = callback;

    QJsonObject secondRequest = mewbassRequest["request"].toObject();
    QJsonArray payments = secondRequest["payments"].toArray();

    for (QJsonValue itemJson : secondRequest["items"].toArray())
    {
        Product item = Product::parse(itemJson);
        productsSum += item.price;
    }

    recievedTask.type = secondRequest["type"].toString() == "sellReturn" ? OPERATIONS_Return : OPERATIONS_Sale;
    double paymentSum = {0};

    for ( QJsonValue paymentValue : payments )
    {
        QJsonObject payment = paymentValue.toObject();
        paymentSum += payment["sum"].toDouble();

        if ( payment["type"].toString() == "1" )
        {
            recievedTask.ePay = true;
            recievedTask.ePaySum += payment["sum"].toDouble();
        }
    }

    if ( productsSum > paymentSum )
    {
        recievedTask.isValid = false;
    }

    return recievedTask;
}
