#include "cell.h"

Cell::Cell(QWidget* parent)
    : QPushButton(parent)
{
    // style
    setFixedSize(30, 30);
    setStyleSheet("Cell\
        {\
            border: 1px solid darkgray;\
            background: black;\
        }\
    ");
}

bool Cell::hasMine() const
{
    return m_hasMine;
}

void Cell::setHasMine(bool hasMine)
{
    m_hasMine = hasMine;
}

int Cell::digit()
{
    return m_digit;
}

void Cell::setDigit(int digit)
{
    m_digit = digit;
}

bool Cell::hasFlag()
{
    return m_hasFlag;
}

void Cell::setFlag(bool flag)
{
    m_hasFlag = flag;
}

void Cell::setRow(int row)
{
    m_row = row;
}

void Cell::setCol(int col)
{
    m_col = col;
}

int Cell::row() const
{
    return m_row;
}

int Cell::col() const
{
    return m_col;
}

Cell::States Cell::state()
{
    return m_state;
}

void Cell::setState(States state)
{
    m_state = state;
}

void Cell::reveal()
{
    setStyleSheet("\
        Cell\
        {\
              border: 1px solid darkgray;\
              background: grey;\
        }\
    ");

    // check if win
    if (!hasMine())
        emit cellRevealed();

    auto prev_state = state();

    m_state = revealed;
    if (hasMine()) {
        setIcon(QIcon(QPixmap(":/img/img/bomb.png")));
        setIconSize(QSize(30, 30));
    } else if (digit() != 0){
      setText(QString::number(digit()));
    } else {
        setText("");
    }

    if (prev_state == flagged) {
        emit flagsCountChanged();
        setIcon(QIcon());
    }
}

void Cell::setDefault()
{
    setStyleSheet("Cell\
        {\
            border: 1px solid darkgray;\
            background: black;\
        }\
    ");

    setText("");
    setEnabled(true);

    m_hasMine = false;
    m_hasFlag = false;
    m_digit = false;
    m_state = hidden;
}

void Cell::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && state() != revealed && state() != flagged) {
        if (digit() == 0 && !hasMine()) {
            reveal();
            emit zeroDigit(m_row, m_col);
        } else if (hasMine()) {
            emit detonate();
        } else {
            reveal();
        }
    } else if (event->button() == Qt::RightButton){
        if (state() == hidden) {
            setIcon(QIcon(QPixmap(":/img/img/flag.png")));
            setIconSize(QSize(30, 30));
            setState(flagged);
            emit flagsCountChanged();
        } else if (state() != revealed){
            setIcon(QIcon());
            setState(hidden);
            emit flagsCountChanged();
        }
    }
}
