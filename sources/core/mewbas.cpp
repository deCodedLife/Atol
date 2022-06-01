#include "mewbas.h"

Mewbas::Mewbas(QObject *parent) : TObject(parent)
{
    RequestDaemon.SetTicks(5);
    RequestDaemon.Reset();

    logList = QMap<GLOBAL_ERRORS, Log> {
        { API_SIDE_ERROR, Log{"MEWBAS", "ERROR", "Got error from api side", ""} },
        { WRONG_JSON_DATA, Log{"MEWBAS", "ERROR", "Can't parse response", ""} }
    };

    connect(&m_net, &Network::dataRecived, this, &Mewbas::gotData);
    connect(&RequestDaemon, &Daemon::triggered, this, &Mewbas::updatePayments);
    connect(&RequestDaemon, &Daemon::triggered, this, &Mewbas::updateOperations);
}

void Mewbas::SendRequest(Request r)
{
    m_net.Post(m_configuration.mewbasAddr, r.toJson());
}

void Mewbas::ChangeStatus(Task task)
{
    if ( task.isOperation )
    {
        NextTask();
        return;
    }

    QString status;
    status = task.status == TASK_SUCCSESS ? "done" : "error";

    Request mewRequest = Request::StatusChange(task.saleID, status, task.description);
    mewRequest.ChangeJWT(m_configuration.jwt);

    SendRequest( mewRequest );
    NextTask();
}

void Mewbas::NextTask()
{
    m_currentTask = QJsonObject();
}

void Mewbas::updatePayments()
{
    if ( m_currentTask.isEmpty() == false )
    {
        return;
    }

    Request mewRequest = Request::GetPayments(m_configuration.cashboxID);
    mewRequest.ChangeJWT(m_configuration.jwt);

    SendRequest(mewRequest);
}

void Mewbas::updateOperations()
{
    if ( m_currentTask.isEmpty() == false )
    {
        return;
    }

    Request mewRequest = Request::GetOperations(m_configuration.cashboxID);
    mewRequest.ChangeJWT(m_configuration.jwt);

    SendRequest(mewRequest);
}

bool Mewbas::exists(QJsonObject data)
{
    Task current = Task::parse(data);

    // I was too lazy to inplement == operator =(
    for ( Task item : m_queue )
    {
        if ( item.uuid == current.uuid )
        {
            return true;
        }
    }
    // Garbage code

    m_queue.append(current);
    return false;
}

void Mewbas::gotData(QString data)
{
    QJsonParseError error;
    QJsonObject mewbasResponse = QJsonDocument::fromJson(data.toUtf8(), &error).object();

    if ( error.error != QJsonParseError::NoError )
    {
        Log wrongData = logList[WRONG_JSON_DATA];
        wrongData.details = error.errorString();
        Logger::WriteToFile(wrongData);

        return;
    }

    if ( mewbasResponse["status"].isNull() == false && mewbasResponse["status"].toInt() != 200 )
    {
        if ( mewbasResponse["status"].toString().toInt() != 200 )
        {
            if ( mewbasResponse["data"].toString().trimmed() == "" )
            {
                return;
            }

            Log errLog = logList[API_SIDE_ERROR];
            errLog.details = mewbasResponse["data"].toString();
            Logger::WriteToFile(errLog);

            return;
        }
    }

    if ( mewbasResponse.isEmpty() ) return;
    if ( mewbasResponse["data"].isArray() ) return;
    if ( mewbasResponse["data"].toBool() == true && mewbasResponse["data"].toObject().isEmpty() ) return;

    if (m_currentTask.isEmpty() == false || exists(mewbasResponse))
    {
        return;
    }

    if (Task::parse(mewbasResponse).isValid == false)
    {
        return;
    }

    m_currentTask = mewbasResponse;
    emit newTask(m_currentTask);
}
