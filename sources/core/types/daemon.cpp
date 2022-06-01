#include "daemon.h"

Daemon::Daemon(QObject *parent) : QObject(parent)
{

}

void Daemon::SetTicks(int t)
{
    m_ticksCount = t;
}

void Daemon::Stop()
{
    m_shutedDown = true;
}

void Daemon::Reset()
{
    m_ticksExpired = m_ticksCount;
    m_shutedDown = false;
}

void Daemon::Countdown()
{
    if ( m_shutedDown )
    {
        return;
    }

    m_ticksExpired--;

    if ( m_ticksExpired <= 0 )
    {
        Reset();
        emit triggered("Timeout");
    }
}
