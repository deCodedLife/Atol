#pragma once

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>

#define CALLBACK_URL "http://localhost:7084"

enum OperationTypes
{
    OPERATIONS_None = -1,
    OPERATIONS_Sale,
    OPERATIONS_Return,
    OPERATIONS_Operation
};

enum Statuses
{
    TASK_NONE,
    TASK_FAIL,
    TASK_SUCCSESS
};

struct Product
{
    double amount;
    QString name;
    QString paymentObject;
    bool piece;
    double price;
    int quantity;
    QJsonObject tax;
    QString type;

    static Product parse(QJsonValue);
};


struct Task
{
    int saleID;
    double ePaySum;

    QString uuid;
    QString description;

    bool ePay;
    bool isOperation;
    bool isValid;

    QJsonObject task;

    OperationTypes type;
    Statuses status;

    static Task parse(QJsonObject);

};
