#include "server.h"

Server::Server(QObject *parent) : TObject(parent)
{
    TimeoutDaemon.SetTicks(40);
    EpayTimeout.SetTicks(1);

    logList = QMap<GLOBAL_ERRORS, Log> {
        {CANCELED_BY_OPERATOR, Log{"SERVER", "ERRPR", "Task canceled", "Task was canceled by operator"}},
        {SENDED_TO_PRINT, Log{"SERVER", "INFO", "Task was sended to print.", ""}},
        {ATOL_RESPOND, Log{"SERVER", "INFO", "Atol responded: ", ""}},
        {ATOL_ERROR_RESPOND, Log{"SERVER", "ERROR", "Atol reported an error", ""}},
        {TASK_FAILED, Log{"SERVER", "ERROR", "Task failed", ""}},
        {ATOL_CASHBOX_ERROR, Log{"SERVER", "ERROR", "Got responce from cashbox", ""}}
    };

#if (AUTO_CONFIRM)
    connect(&worker, &HttpWorker::dataRecived, this, &Server::AtolRecived);
#endif
    connect(&eCash, &Terminal::succsess, this, &Server::printCashcheck);
    connect(&eCash, &Terminal::timeout, this, &Server::OperationFailed);
    connect(&eCash, &Terminal::gotError, this, &Server::OperationFailed);
    connect(&eCash, &Terminal::codeDetected, this, &Server::handleRecieptCode);

    connect(&net, &Network::dataRecived, this, &Server::GotResponse);
    connect(&net, &Network::timeout, this, &Server::OperationFailed);

    connect(this, &Server::taskEnded, &EpayTimeout, &Daemon::Stop);
    connect(this, &Server::taskEnded, &TimeoutDaemon, &Daemon::Stop);

    connect(&TimeoutDaemon, &Daemon::triggered, this, &Server::OperationFailed);
    connect(&EpayTimeout, &Daemon::triggered, &eCash, &Terminal::CheckStatus);
    connect(this, &Server::configsUpdated, &eCash, &Terminal::UpdateConfig);

    eCash.UpdateConfig(m_configuration);
}

QString Server::qmlGetStatus()
{
    return currentStatus;
}

void Server::qmlCancelOperation()
{
    currentTask.description = logList[CANCELED_BY_OPERATOR].details;
    Logger::WriteToFile(logList[CANCELED_BY_OPERATOR]);

    cancelOperation();
}

void Server::GotTask(QJsonObject data)
{
    currentTask = Task::parse(data);

    if ( currentTask.ePay )
    {
        if ( currentTask.type == OPERATIONS_Return )
        {
            currentStatus ="Введите код на терминале: " + currentTask.returnCode;
        }
        else
        {
            currentStatus = TERMINAL_STATUS;
        }

        state = TERMINAL;
        emit statusChanged(currentStatus);

        eCash.Pay( currentTask.ePaySum, currentTask.type );
        EpayTimeout.Reset();

        return;
    }

    printCashcheck();
}

void Server::printCashcheck()
{
    state = CASHBOX;
    TimeoutDaemon.Reset();

    if (currentTask.isOperation == false)
    {
        currentStatus = CASHBOX_STATUS;
        emit statusChanged(currentStatus);
    }

    Log infoLog = logList[SENDED_TO_PRINT];
    infoLog.details = QJsonDocument(currentTask.task).toJson();
    Logger::WriteToFile(infoLog);

    net.Post(m_configuration.serverAddr + "/api/v2/requests", currentTask.task);

#if (AUTO_CONFIRM)
    worker.SetURL(m_configuration.serverAddr, {"api", "v2", "requests", currentTask.uuid});
#endif
}

void Server::cancelOperation()
{
    currentTask.status = TASK_FAIL;

    if (currentTask.ePay && state == CASHBOX)
    {
        QThread::currentThread()->msleep(1000 * 5);
        eCash.Pay(currentTask.ePaySum, OPERATIONS_Return);
    }

    state = STATE_NONE;

    emit updateStatus(currentTask);
    emit taskEnded();
}

void Server::handleRecieptCode(QString code)
{
    emit updateRecieptCode(currentTask, code);
}

QString Server::QMLcashboxStatus()
{
    return CASHBOX_STATUS;
}

QString Server::QMLterminalStatus()
{
    return TERMINAL_STATUS;
}

void Server::GotResponse(QString data)
{
    Log atolLog;
    QJsonObject responce = QJsonDocument::fromJson(data.toUtf8()).object();

    if ( responce["uuid"].isNull() == false )
    {
        atolLog = logList[ATOL_RESPOND];
        atolLog.details = data;

//        currentTask.status = TASK_SUCCSESS;
//        currentTask.description = "Ошибок нет";
        Logger::WriteToFile(atolLog);

#if (AUTO_CONFIRM)
        worker.Stop();
#endif
//        emit taskEnded();
//        emit updateStatus(currentTask);

        return;
    }

    if ( responce["error"].isNull() == false )
    {
        QJsonObject error = responce["error"].toObject();
        currentTask.description = error["description"].toString();

        atolLog = logList[ATOL_ERROR_RESPOND];
        atolLog.details = error["description"].toString();

        Logger::WriteToFile(atolLog);
        cancelOperation();
    }
}

void Server::OperationFailed(QString description)
{
    currentTask.status = TASK_FAIL;
    currentTask.description = description;

    Log errLog = logList[TASK_FAILED];
    errLog.details = currentTask.description;

    Logger::WriteToFile(errLog);
    cancelOperation();
}

void Server::HttpWorkerRecived(QString from, QJsonObject response)
{
    QList<QString> urlKeys = from.split('/');

    if (urlKeys.last() != currentTask.uuid || currentTask.status != TASK_NONE)
    {
        return;
    }

    if ( response["results"].isNull() )
    {
        return;
    }

#if (AUTO_CONFIRM)
    worker.Stop();
#endif

    AtolRecived(response);
}

void Server::AtolRecived(QJsonObject response)
{
    QJsonArray results = response["results"].toArray();
    QJsonObject result = results[0].toObject()["error"].toObject();

    currentTask.status = result["code"].toInt() == 0 ? TASK_SUCCSESS : TASK_FAIL;
    currentTask.description = result["description"].toString();

    if ( results[0].toObject()["status"] != "ready" && results[0].toObject()["status"] != "error" )
    {
        if ( results[0].toObject()["status"] != "wait" && results[0].toObject()["status"] != "inProgress" )
        {
            Log httpLog = logList[ATOL_CASHBOX_ERROR];
            httpLog.details = currentTask.description;
            Logger::WriteToFile(httpLog);

            currentTask.status = TASK_FAIL;
            cancelOperation();
        }

        return;
    }

    Log httpLog = logList[ATOL_CASHBOX_ERROR];
    httpLog.type = currentTask.status == TASK_SUCCSESS ? "INFO" : "ERROR";
    httpLog.details = currentTask.description;
    Logger::WriteToFile(httpLog);

    if ( currentTask.status == TASK_FAIL )
    {
        cancelOperation();
        return;
    }

    emit taskEnded();
    emit updateStatus(currentTask);
}
