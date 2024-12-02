#include <QGridLayout>
#include <QWidget>
#include <QFrame>
#include <QWindow>
#include "minefield.h"
#include "cell.h"
#include "Random.h"

MineField::MineField(QWidget* parent, int numRows, int numCols, int numMines)
    : QFrame(parent)
    , m_cells(numRows)
    , m_numMines{ numMines }
    , m_flaggedMinesCount(0)
    , m_revealedCells(0)
{
    for (size_t i{ 0 }; i < numRows; ++i)
        m_cells[i].resize(numCols);

    for (auto& arr: m_cells)
        for (auto& e: arr)
            e = new Cell{ this };

    // style
    auto layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void MineField::generateField()
{
    int minesToPlace{ m_numMines };
    const size_t numRows = m_cells.size();
    const size_t numCols = m_cells[0].size();
    
    // place mines at random cells
    while (minesToPlace) {
        size_t randomRow{ Random::get<size_t>(0, numRows - 1) };
        size_t randomCol{ Random::get<size_t>(0, numCols - 1) };
        
        if (!m_cells[randomRow][randomCol]->hasMine()) {
            m_cells[randomRow][randomCol]->setHasMine(true);
            --minesToPlace;
        }
    }
    
    // set digits
    for (size_t row{ 0 }; row < numRows; ++row) {
        for (size_t col{ 0 }; col < numCols; ++col) {
            if (m_cells[row][col]->hasMine()) {
                std::vector<Cell*> adjacentCells{
                    adjacentCellsFor(static_cast<int>(row), static_cast<int>(col)) };
                
                for (auto& cell: adjacentCells)
                    if (!cell->hasMine())
                        cell->setDigit(cell->digit() + 1);
            }
        }
    }

    // add cells to the grid layout and connect signals
    for (size_t i{ 0 }; i < numRows; ++i) {
        for (size_t j{ 0 }; j < numCols; ++j) {
            m_cells[i][j]->setRow(i);
            m_cells[i][j]->setCol(j);
            static_cast<QGridLayout*>(this->layout())->addWidget(m_cells[i][j], i, j);
            QObject::connect(m_cells[i][j], &Cell::zeroDigit, this, &MineField::onZeroDigit);
            QObject::connect(m_cells[i][j], &Cell::detonate, this, &MineField::onDetonate);
            QObject::connect(m_cells[i][j], &Cell::cellRevealed, this, &MineField::onCellRevealed);
        }
    }
}

std::vector<Cell*> MineField::adjacentCellsFor(int row, int col)
{
    std::vector<Cell*> list{};
    const size_t numRows = m_cells.size();
    const size_t numCols = m_cells[0].size();
    
    if (col != 0) // left
        list.push_back(m_cells[row][col - 1]);
    if (col != numCols - 1) // right
        list.push_back(m_cells[row][col + 1]);
    if (row != 0) // upper
        list.push_back(m_cells[row - 1][col]);
    if (row != numRows - 1) // bottom
        list.push_back(m_cells[row + 1][col]);
    if (col != 0 && row != 0) // upper-left diagonal
        list.push_back(m_cells[row - 1][col - 1]);
    if (col != numCols - 1 && row != 0) // upper-right diagonal
        list.push_back(m_cells[row - 1][col + 1]);
    if (col != numCols - 1 && row != numRows - 1) // bottom-right diagonal
        list.push_back(m_cells[row + 1][col + 1]);
    if (col != 0 && row != numRows - 1) // bottom-left diagonal
        list.push_back(m_cells[row + 1][col - 1]);
    
    return list;
}

void MineField::onZeroDigit(int row, int col)
{
    auto adjacentCells{ adjacentCellsFor(row, col) };

    for (auto& e: adjacentCells) {
        if (e->state() == Cell::revealed)
            continue;
        if (e->digit() == 0 && !e->hasMine()) {
            e->reveal();
            onZeroDigit(e->row(), e->col());
        } else if (!e->hasMine()){
            e->reveal();
        }
    }
}

void MineField::onDetonate()
{
    for (auto& arr: m_cells) {
        for (auto& e: arr) {
            e->setEnabled(false);
            if (e->hasMine())
                e->reveal();
        }
    }
    emit lose();
}

void MineField::setFlagCounter(FlagCounter *flagcounter)
{
    flagcounter->setFlagsCount(m_numMines);
    for (auto& arr: m_cells)
        for (auto& c: arr)
            QObject::connect(c, &Cell::flagsCountChanged, flagcounter, &FlagCounter::onFlagsCountChanged);
}

int MineField::numMines()
{
    return m_numMines;
}

void MineField::onCellRevealed()
{
    int numRows = m_cells.size();
    int numCols = m_cells[0].size();
    int minesToBeRevealed{ (numRows * numCols) - m_numMines };

    ++m_revealedCells;
    if (m_revealedCells == minesToBeRevealed) {
        emit win();
    }
}
