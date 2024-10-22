#include "timer.h"

Timer::Timer(QWidget* parent)
    : QLCDNumber{ parent }, m_timer{ parent }
{
    setDigitCount(4);
    setStyleSheet("QLCDNumber { background-color: black; } ");

    m_timer.setInterval(1000);
    QObject::connect(&m_timer, &QTimer::timeout, this, &Timer::updateTimer);
    start();
}

void Timer::updateTimer()
{
    display(++m_seconds);
}

void Timer::setTimer(int seconds)
{
    m_seconds = seconds;
}

int Timer::seconds()
{
    return m_seconds;
}

void Timer::start()
{
    m_timer.start();
}

void Timer::stop()
{
    m_timer.stop();
}
