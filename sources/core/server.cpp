#include "server.h"

Server::Server(QObject *parent) : TObject(parent)
{
    m_isBlocked = false;
    EpayTimeout.SetTicks(1);

    fast_fix.SetTime(1000);
    fast_fix.configure(fast_fix_thread);
    fast_fix.moveToThread(&fast_fix_thread);

    fast_fix_thread.start();

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
//    connect(&eCash, &Terminal::timeout, this, &Server::OperationFailed);
    connect(&eCash, &Terminal::gotError, this, &Server::OperationFailed);
    connect(&eCash, &Terminal::codeDetected, this, &Server::handleRecieptCode);

    connect(&net, &Network::dataRecived, this, &Server::GotResponse);
    connect(&net, &Network::timeout, this, &Server::OperationFailed);

    connect(this, &Server::taskEnded, &EpayTimeout, &Daemon::Stop);
    connect(this, &Server::taskEnded, &TimeoutDaemon, &Daemon::Stop);

//    connect(&TimeoutDaemon, &Daemon::triggered, this, &Server::OperationFailed);
//    connect(&TimeoutDaemon, &Daemon::triggered, this, &Server::cancelPrint);
    connect(&EpayTimeout, &Daemon::triggered, &eCash, &Terminal::CheckStatus);
//    connect(&fast_fix, &Timer::timeout, &eCash, &Terminal::CheckStatus);

//    connect(&fast_fix, &Timer::timeout, &fast_fix, &Timer::Resume);

    connect(this, &Server::configsUpdated, &eCash, &Terminal::UpdateConfig);

    eCash.UpdateConfig(m_configuration);
}

void Server::qmlReturnEpay(double sum)
{
    eCash.Pay(sum, OPERATIONS_Return);
}

void Server::cancelPrint()
{
    m_isBlocked = true;
    net.Delete(m_configuration.serverAddr + "/api/v2/requests/" + currentTask.uuid);

    emit notify( "Ошибка", "Соединение с кассой потеряно. Не проводите операции до устранения неполадок" );
    emit block();
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

    if ( currentTask.ePay && ( currentTask.pay_method == "card" || currentTask.pay_method == "parts" ) )
    {
        if ( currentTask.type == OPERATIONS_Return )
        {
            currentStatus = "Введите код на терминале: " + currentTask.returnCode;
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
}

void Server::cancelOperation()
{
    if ( currentTask.status == TASK_SUCCSESS )
    {
        return;
    }
    currentTask.status = TASK_FAIL;

//    if (currentTask.ePay && state == CASHBOX)
//    {
//        QThread::currentThread()->msleep(1000 * 5);
//        eCash.Pay(currentTask.ePaySum, OPERATIONS_Return);
//    }

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
    QJsonObject response = QJsonDocument::fromJson(data.toUtf8()).object();

    if ( response.contains( "uuid" ) )
    {
        atolLog = logList[ATOL_RESPOND];
        atolLog.details = data;

        Logger::WriteToFile(atolLog);
    }

    if ( response.contains( "error" ) && currentTask.status != TASK_SUCCSESS )
    {
        QJsonObject error = response[ "error" ].toObject();

        if ( error[ "code" ].toInt() != 505 )
        {
            currentTask.description = "{ERROR} [" + error[ "code" ].toString() + "] " + error["description"].toString();

            atolLog = logList[ATOL_ERROR_RESPOND];
            atolLog.details = error["description"].toString();

            Logger::WriteToFile(atolLog);
            cancelOperation();
        }
    }

    currentTask.description = "done";
    currentTask.status = TASK_SUCCSESS;

    emit updateStatus(currentTask);
    emit taskEnded();
}

void Server::OperationFailed(QString description)
{
    currentTask.status = TASK_FAIL;
    currentTask.description = "Atol: " + description;

    Log errLog = logList[TASK_FAILED];
    errLog.details = "Atol: " + currentTask.description;

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

//    return;

//    if ( m_isBlocked )
//    {
//        return;
//    }

//    QJsonArray results = response["results"].toArray();
//    QJsonObject result = results[0].toObject()["error"].toObject();

//    currentTask.status = result["code"].toInt() == 0 ? TASK_SUCCSESS : TASK_FAIL;
//    currentTask.description = result["description"].toString();

//    if ( results[0].toObject()["status"] != "ready" && results[0].toObject()["status"] != "error" )
//    {
//        if ( results[0].toObject()["status"] != "wait" && results[0].toObject()["status"] != "inProgress" )
//        {
//            Log httpLog = logList[ATOL_CASHBOX_ERROR];
//            httpLog.details = currentTask.description;
//            Logger::WriteToFile(httpLog);

//            currentTask.status = TASK_FAIL;
//            cancelOperation();
//        }

//        return;
//    }

//    Log httpLog = logList[ATOL_CASHBOX_ERROR];
//    httpLog.type = currentTask.status == TASK_SUCCSESS ? "INFO" : "ERROR";
//    httpLog.details = currentTask.description;
//    Logger::WriteToFile(httpLog);

//    if ( currentTask.status == TASK_FAIL )
//    {
//        cancelOperation();
//        return;
//    }

//    emit taskEnded();
//    emit updateStatus(currentTask);
}

void Server::qmlRestoreServer()
{
    emit restore();
}
