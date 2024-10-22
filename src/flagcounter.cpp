#include "flagcounter.h"
#include "cell.h"

FlagCounter::FlagCounter(QWidget* parent)
    : QLCDNumber(parent)
{
    setDigitCount(3);
    setStyleSheet("QLCDNumber { background-color: black; }");
}

void FlagCounter::setFlagsCount(int flagsCount)
{
    m_flagsCount = flagsCount;
    display(m_flagsCount);
}

int FlagCounter::flagsCount()
{
    return m_flagsCount;
}

void FlagCounter::onFlagsCountChanged()
{
    if (static_cast<Cell*>(sender())->state() == Cell::hidden) {
        setFlagsCount(flagsCount() + 1);
    } else if (static_cast<Cell*>(sender())->state() == Cell::revealed) {
        setFlagsCount(flagsCount() + 1);
    } else {
        setFlagsCount(flagsCount() - 1);
    }
}
