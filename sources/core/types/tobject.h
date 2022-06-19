#pragma once

#include <QObject>

#include "logs.h"
#include "task.h"
#include "configs.h"
#include "request.h"

enum GLOBAL_ERRORS
{
    API_SIDE_ERROR,
    WRONG_JSON_DATA,
    CANCELED_BY_OPERATOR,
    SENDED_TO_PRINT,
    ATOL_RESPOND,
    ATOL_ERROR_RESPOND,
    TASK_FAILED,
    ATOL_CASHBOX_ERROR,
    CANT_OPEN_FILE,
    CONTENT_IS_EMPTY
};

class TObject : public QObject
{
    Q_OBJECT
signals:
    void configsUpdated(Configuration);

public:
    explicit TObject(QObject *parent = nullptr);

public slots:
    void UpdateConfig(Configuration);

protected:
    QMap<GLOBAL_ERRORS, Log> logList;
    Configuration m_configuration;
};

