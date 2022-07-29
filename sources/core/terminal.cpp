#include "terminal.h"

Terminal::Terminal(QObject *parent) : TObject(parent)
{
    connect(this, &TObject::configsUpdated, &m_terminalParser, [&] (Configuration configuration) {
        m_terminalParser.SetDirectory(configuration.terminalDir);
    });
    connect(this, &Terminal::succsess, &m_recieptParser, &ReceiptParser::StartParsing);
    connect(&m_recieptParser, &ReceiptParser::codeDetected, this, &Terminal::codeDetected);
}

void Terminal::Pay(double sum, OperationTypes type)
{
    m_payment = (int) (sum * 100);
    m_timeOut = 60 * 2;

    QTime currentTime = QTime::currentTime();
    currentTime = currentTime.addSecs(-10);

    m_terminalParser.SetTarget({m_payment, currentTime, PAYMENT_NOTFOUND});
    m_recieptParser.SetTarget(m_configuration.terminalDir + "/P");

    QStringList arguments;
    arguments.append(QString::number(m_payment));

    QString file = m_configuration.terminalDir;

    if ( type == OPERATIONS_Sale )
    {
        file += "/pay.bat";
    }

    else if ( type == OPERATIONS_Cancel )
    {
        file += "/cancel.bat";
    }

    else if ( type == OPERATIONS_Return )
    {
        file += "/return.bat";
    }

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
    TPayment currentPayment = m_terminalParser.Parse();

    if ( m_timeOut < 1 )
    {
        Cancel();
        emit timeout("[ERROR] [EPAY] Terminal timeout");
    }

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
}

void Terminal::StartTimeout()
{
    m_isActive = true;
}

void Terminal::Cancel()
{
    this->m_isActive = false;
}
