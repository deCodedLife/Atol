#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QThread>

class Timer : public QObject
{
    Q_OBJECT
public:
    explicit Timer(int timeMS = 1000, QObject *parent = nullptr);
    void configure(QThread &);
    void SetTime(int);

    void Resume();
    void Pause();

    bool GetState();

signals:
    void timeout();

public slots:
    void timer();

private:
    int timeMs;
    bool isWorking;
};

#endif // TIMER_H
