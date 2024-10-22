#include <QGridLayout>
#include <QMessageBox>
#include <QDockWidget>
#include <QPushButton>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QAction>
#include <QWindow>
#include <QTimer>
#include <QLabel>
#include <QString>
#include <QMenuBar>
#include "mainwindow.h"
#include "minefield.h"
#include "cell.h"
#include "flagcounter.h"
#include "timer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // minefield creation
    m_minefield = new MineField{this, 8, 8, 10};
    m_minefield->generateField();

    // flag counter
    m_flagCounter = new FlagCounter{ this };
    m_minefield->setFlagCounter(m_flagCounter);

    // win and lose
    QObject::connect(m_minefield, &MineField::win, this, &MainWindow::onWin);
    QObject::connect(m_minefield, &MineField::lose, this, &MainWindow::onLose);

    // timer
    m_timer = new Timer{ this };

    // set layout of the main window
    setupLayout();

    // set toolbar menu
    setupToolBarMenu();
}

MainWindow::~MainWindow()
{
    delete m_minefield;
    delete m_mainFrameLayout;
    delete m_flagCounter;
    delete m_timer;
}

void MainWindow::newGame(int numRows, int numCols, int numMines)
{
    delete m_minefield;

    m_minefield = new MineField{ this, numRows, numCols, numMines };
    m_minefield->generateField();

    m_mainFrameLayout->addWidget(m_minefield);

    m_flagCounter->setFlagsCount(numMines);

    m_minefield->setFlagCounter(m_flagCounter);

    m_timer->setTimer(0);
    m_timer->start();

    QObject::connect(m_minefield, &MineField::win, this, &MainWindow::onWin);
    QObject::connect(m_minefield, &MineField::lose, this, &MainWindow::onLose);
}

void MainWindow::setEasyDifficulty()
{
    newGame(8, 8, 10);
}

void MainWindow::setMediumDifficulty()
{
    newGame(16, 16, 40);
}

void MainWindow::setHardDifficulty()
{
    newGame(16, 30, 99);
}

void MainWindow::setupLayout()
{
    auto mainFrame = new QFrame(this);
    m_mainFrameLayout = new QVBoxLayout;
    auto upperLayout = new QHBoxLayout;

    upperLayout->addWidget(m_flagCounter);
    upperLayout->addWidget(m_timer);

    m_mainFrameLayout->addLayout(upperLayout);
    m_mainFrameLayout->addWidget(m_minefield);

    mainFrame->setLayout(m_mainFrameLayout);
    setCentralWidget(mainFrame);

    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

void MainWindow::setupToolBarMenu()
{
    auto easy = new QAction{ "easy", this };
    auto medium = new QAction{ "medium", this };
    auto hard = new QAction{ "hard", this };

    QObject::connect(easy, &QAction::triggered, this, &MainWindow::setEasyDifficulty);
    QObject::connect(medium, &QAction::triggered, this, &MainWindow::setMediumDifficulty);
    QObject::connect(hard, &QAction::triggered, this, &MainWindow::setHardDifficulty);

    auto temp = menuBar()->addMenu("Game");
    temp->addAction(easy);
    temp->addAction(medium);
    temp->addAction(hard);
}

void MainWindow::onWin()
{
    m_timer->stop();

    auto wd = new QWidget;
    auto layout = new QVBoxLayout;
    auto text = new QLabel{ wd };
    text->setText(QString{ "%1s\nTIME\n%2\nCLEARED MINES" }.arg(m_timer->seconds()).arg(m_minefield->numMines()));
    text->setAlignment(Qt::AlignCenter);
    text->setStyleSheet("QLabel {font-size: 30px;}");

    layout->addWidget(text);

    wd->setLayout(layout);

    wd->show();
}

void MainWindow::onLose()
{
    m_timer->stop();
}
