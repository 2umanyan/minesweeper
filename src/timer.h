#ifndef TIMER_H
#define TIMER_H

#include <QLCDNumber>
#include <QWidget>
#include <QTimer>

class Timer: public QLCDNumber
{
    Q_OBJECT
public:
    Timer(QWidget* parent = nullptr);

    void updateTimer();

    void setTimer(int seconds);

    int seconds();

    void start();
    void stop();

private:
    QLCDNumber m_LCDNumber;
    QTimer m_timer;
    int m_seconds;
};

#endif
