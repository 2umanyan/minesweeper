#ifndef FLAGCOUNTER_H
#define FLAGCOUNTER_H

#include <QLCDNumber>
#include <QWidget>

class FlagCounter: public QLCDNumber
{
    Q_OBJECT
public:
    FlagCounter(QWidget* parent = nullptr);

    void setFlagsCount(int flagsCount);

    int flagsCount();

private:
    int m_flagsCount;

public slots:
    void onFlagsCountChanged();
};

#endif
