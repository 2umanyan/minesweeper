#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include "minefield.h"
#include "flagcounter.h"
#include "timer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void newGame(int numRows, int numCols, int numMines);

    void setEasyDifficulty();
    void setMediumDifficulty();
    void setHardDifficulty();

    void setupLayout();

    void setupToolBarMenu();

private:
    MineField* m_minefield;
    QVBoxLayout* m_mainFrameLayout;
    FlagCounter* m_flagCounter;
    Timer* m_timer;

public slots:
    void onWin();
    void onLose();
};
#endif
