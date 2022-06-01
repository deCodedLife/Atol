#include "terminal.h"

Terminal::Terminal(QObject *parent) : TObject(parent)
{
    connect(this, &TObject::configsUpdated, &m_parser, [&] (Configuration configuration) {
        m_parser.SetDirectory(configuration.terminalDir);
    });
}


void Terminal::Pay(double sum, OperationTypes type)
{
    m_payment = (int) (sum * 100);
    m_timeOut = 60 * 2;

    QTime currentTime = QTime::currentTime();
    currentTime = currentTime.addSecs(-10);

    m_parser.SetTarget({m_payment, currentTime, PAYMENT_NOTFOUND});

    QStringList arguments;
    arguments.append(QString::number(m_payment));

    QString file = m_configuration.terminalDir;
    file += type == OPERATIONS_Sale ? "/pay.bat" : "/return.bat";

    QFile::remove(m_configuration.terminalDir + "/P");

    QProcess *procces = new QProcess(this);

    procces->setWorkingDirectory(m_configuration.terminalDir);
    procces->start(file, arguments, QIODevice::ReadWrite);

    if ( procces->error() )
    {
        Log errorLog;
        errorLog.type = "ERROR";
        errorLog.sender = "Terminal";
        errorLog.details = "Can't exec program: " + file;
        errorLog.data = procces->errorString();
    }

    Log testLog;
    testLog.type = "INFO";
    testLog.sender = "TERMINAL";
    testLog.data = "sended to print";
    testLog.details = QString::number(m_payment) + " " + currentTime.toString() + " ";

    StartTimeout();
}

void Terminal::Validate()
{
    QStringList arguments;
    arguments.append("7");

    QString file = m_configuration.terminalDir + "/loadparm.exe";
    QProcess *process = new QProcess(this);

    m_procces->setWorkingDirectory(m_configuration.terminalDir);
    m_procces->start(file, arguments, QIODevice::ReadWrite);
}

void Terminal::CheckStatus()
{
    if ( this->m_isActive == false )
    {
        return;
    }

    m_timeOut--;
    TPayment currentPayment = m_parser.Parse();

    if ( currentPayment.Result == PAYMENT_NOTFOUND )
    {
        return;
    }

    if ( currentPayment.Result == PAYMENT_ERROR )
    {
        Cancel();
        emit gotError("Problems with card");
    }

    if ( currentPayment.Result == PAYMENT_DONE )
    {
        Cancel();
        emit succsess();
    }

    if ( m_timeOut < 1 )
    {
        Cancel();
        emit timeout("[ERROR] [EPAY] Terminal timeout");
    }
}

void Terminal::StartTimeout()
{
    m_isActive = true;
}

void Terminal::Cancel()
{
    this->m_isActive = false;
}
