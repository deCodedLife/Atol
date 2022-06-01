#pragma once

#include <QObject>

class Daemon : public QObject
{
    Q_OBJECT
signals:
    void triggered(QString);

public:
    explicit Daemon(QObject *parent = nullptr);
    void SetTicks(int);
    void Stop();
    void Reset();

public slots:
    void Countdown();

private:
    bool m_shutedDown;

    int m_ticksCount;
    int m_ticksExpired;
};

