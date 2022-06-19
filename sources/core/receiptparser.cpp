#include "receiptparser.h"

ReceiptParser::ReceiptParser(QObject *parent) : TObject(parent)
{
    logList = QMap<GLOBAL_ERRORS, Log> {
        {CANT_OPEN_FILE, Log{"RecieptParser", "Error", "Can't open file", ""}},
        {CONTENT_IS_EMPTY, Log{"RecieptParser", "Error", "Can't parse file", "Parsed file has no correct value"}}
    };
}

void ReceiptParser::StartParsing()
{
    QFile file(m_target);
    file.open(QIODevice::ReadOnly);

    if ( file.isOpen() == false )
    {
        Log errorLog = logList[CANT_OPEN_FILE];
        errorLog.details = file.errorString();
        Logger::WriteToFile(errorLog);

        return;
    }

    parse(file.readAll());
    file.close();
}

void ReceiptParser::SetTarget(QString file)
{
    m_target = file;
}

void ReceiptParser::parse(QString data)
{
    QStringList recieptSliced = data.split(PATTERN);
    QStringList codeSliced = recieptSliced.last().split('\n');

    if ( codeSliced.length() < 2 ) {
        Logger::WriteToFile(logList[CONTENT_IS_EMPTY]);
        return;
    }

    codeSliced = codeSliced.first().split('\r');
    emit codeDetected(codeSliced.first().trimmed());
}
