#ifndef cell_h
#define cell_h

#include <QPushButton>
#include <QString>
#include <QObject>
#include <QStyle>
#include <QMouseEvent>

class Cell: public QPushButton {
    Q_OBJECT
public:
    enum States
    {
        hidden,
        revealed,
        flagged
    };

    Cell(QWidget* parent = nullptr);

    bool hasMine() const;
    void setHasMine(bool hasMine);

    int digit();
    void setDigit(int digit);

    bool hasFlag();
    void setFlag(bool flag);

    void setRow(int row);
    void setCol(int col);

    int row() const;
    int col() const;

    States state();

    void setState(States state);

    // reveals cell
    void reveal();

    void setDefault();

    void mousePressEvent(QMouseEvent *event) override;

private:
    bool m_hasMine{ false };
    bool m_hasFlag{ false };
    int m_digit{ 0 };
    States m_state{ hidden };
    int m_row;
    int m_col;

signals:
    // emits when bomb is revealed
    void detonate();

    //emits when 0 digit is revealed
    void zeroDigit(int row, int col);

    void flagsCountChanged();

    // emits when cell is revealed
    void cellRevealed();
};

#endif
