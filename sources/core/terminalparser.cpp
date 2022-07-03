#include "terminalparser.h"



TerminalParser::TerminalParser(QObject *parent) : TObject(parent)
{
    logList = QMap<GLOBAL_ERRORS, Log> {
        {CANT_OPEN_FILE, {"LOGPARSER", "ERROR", "Can't open file", ""}}
    };

    m_actions = {"Command", "Result"};
}

void TerminalParser::SetTarget(TPayment payment)
{
    m_currentPayment = payment;
}

void TerminalParser::SetDirectory(QString folder)
{
    QString logID = QDate::currentDate().toString("yyMM");
    m_lastLog = folder.append("/sbkernel").append(logID).append(".log");
    m_terminalFolder = folder;
}

TPayment TerminalParser::Parse()
{
    QFile terminalLogFile(m_lastLog);
    terminalLogFile.open(QIODevice::ReadOnly);

    if ( terminalLogFile.isOpen() == false )
    {
        Log fileOpenError = logList[CANT_OPEN_FILE];
        fileOpenError.details = terminalLogFile.errorString();

        Logger::WriteToFile(fileOpenError);
    }

    TPayment last;
    QTextStream logTextStream(&terminalLogFile);

    last.Result = PAYMENT_NOTFOUND;
    bool applyResult = false;

    while ( logTextStream.atEnd() == false )
    {
        TLine tempPayment;
        QString line = logTextStream.readLine();

        if ( isValid(line) == false )
        {
            continue;
        }

        tempPayment = TLine::fromString(line);

        if ( tempPayment.type == LINE_COMMAND )
        {

            if ( isTarget(tempPayment) == false )
            {
                applyResult = false;
                continue;
            }

            applyResult = true;
            continue;

        }

        if ( tempPayment.type == LINE_RESULT )
        {

            if ( applyResult == false )
            {
                continue;
            }

            if ( tempPayment.Value != "0" )
            {
                last.Result = PAYMENT_ERROR;
                continue;
            }

        }

        last.Result = PAYMENT_DONE;
    }

    terminalLogFile.close();
    return last;
}

bool TerminalParser::isValid(QString line)
{
    QString currentDate = QDate::currentDate().toString("dd.MM");
    TLine parsed = TLine::fromString(line);

    if ( parsed.Date != currentDate )
    {
        return false;
    }

    if ( parsed.Sender != "SBKRNL:" )
    {
        return false;
    }

    if ( parsed.type == LINE_NONE )
    {
        return false;
    }

    QTime operationTime = QTime::fromString(parsed.Time, "HH:mm:ss");

    if ( operationTime < m_currentPayment.After )
    {
        return false;
    }

    return true;
}

bool TerminalParser::isTarget(TLine payment)
{
    if ( payment.Action != m_actions.first()  )
    {
        return false;
    }

    if ( payment.Value.toInt() != m_currentPayment.TargetSum )
    {
        return false;
    }

    return true;
}

void TerminalParser::changeResult(TLine line, TPayment &payment)
{
    if ( line.Value.toInt() != 0 )
    {
        payment.Result = PAYMENT_ERROR;
    }

}

TLine TLine::fromString(QString data)
{
    TLine payment;
    QStringList logValues = data.split(" ");

    payment.type = LINE_NONE;

    if ( logValues.length() < 4 )
    {
        return payment;
    }

    payment.Date = logValues.first();
    payment.Time = logValues.at(1);
    payment.Sender = logValues.at(2);
    payment.Action = logValues.at(3);
    payment.Value = logValues.last();

    if ( payment.Action == TYPE_COMMAND )
    {
        payment.type = LINE_COMMAND;
    }

    if ( payment.Action == TYPE_RESULT )
    {
        payment.type = LINE_RESULT;
    }

    return payment;
}
