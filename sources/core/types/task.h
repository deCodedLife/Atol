#pragma once

#include <QObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

#define CALLBACK_URL "http://localhost:7084"

enum OperationTypes
{
    OPERATIONS_None = -1,
    OPERATIONS_Sale,
    OPERATIONS_Return,
    OPERATIONS_Operation,
    OPERATIONS_Cancel
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
    QString hash;
    QString description;
    QString returnCode;
    QString pay_method;

    bool ePay;
    bool isOperation;
    bool isValid;

    QJsonObject task;
    QList<int> sales;

    OperationTypes type;
    Statuses status;

    static Task parse(QJsonObject);
};
