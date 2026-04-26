#include "mainwindow.h"
#include "homescreen.h"
#include "gamescreen.h"
#include <QStackedWidget>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    homeScreen = new HomeScreen(this);
    gameScreen = new GameScreen(this);

    stackedWidget->addWidget(homeScreen);
    stackedWidget->addWidget(gameScreen);

    connect(homeScreen, &HomeScreen::startGameClicked, this, &MainWindow::onStartGame);
    connect(homeScreen, &HomeScreen::exitClicked, this, &MainWindow::onExit);
    connect(gameScreen, &GameScreen::backToHomeClicked, this, &MainWindow::onBackToHome);

    setWindowTitle("Sudoku Game");
    resize(800, 600);
}

MainWindow::~MainWindow() {}

void MainWindow::onStartGame()
{
    QString difficulty = homeScreen->selectedDifficulty();
    gameScreen->startNewGame(difficulty);
    stackedWidget->setCurrentWidget(gameScreen);
}

void MainWindow::onExit()
{
    QApplication::quit();
}

void MainWindow::onBackToHome()
{
    stackedWidget->setCurrentWidget(homeScreen);
}
