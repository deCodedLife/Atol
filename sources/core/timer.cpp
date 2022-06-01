#include "timer.h"

Timer::Timer(int time, QObject *parent) : QObject(parent), timeMs(time), isWorking(true) {}

void Timer::configure(QThread &thread)
{
    QObject::connect(&thread, &QThread::started, this, &Timer::timer);
}

void Timer::SetTime(int time)
{
    timeMs = time;
}

void Timer::Resume()
{
    isWorking = true;
}

void Timer::Pause()
{
    isWorking = false;
}

bool Timer::GetState()
{
    return isWorking;
}

void Timer::timer()
{
    while (1) {
        if ( isWorking == false )
        {
            return;
        }

        QThread::msleep(timeMs);
        emit timeout();
    }
}
