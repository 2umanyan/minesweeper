#ifndef minefield_h
#define minefield_h

#include <vector>
#include <QFrame>
#include <QWidget>
#include "cell.h"
#include "flagcounter.h"

class MineField: public QFrame {
    Q_OBJECT
public:
    MineField(QWidget* parent, int numRows, int numCols, int numMines);

    void generateField();

    std::vector<Cell*> adjacentCellsFor(int row, int col);

    void setFlagCounter(FlagCounter* flagcounter);

    int numMines();

private:
    std::vector<std::vector<Cell*>> m_cells;
    int m_numMines;
    int m_flaggedMinesCount;
    int m_revealedCells;

public slots:
    void onZeroDigit(int row, int col);

    void onDetonate();

    void onCellRevealed();

signals:
    void win();
    void lose();
};

#endif
