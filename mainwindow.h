#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QStackedWidget;
class HomeScreen;
class GameScreen;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartGame();
    void onExit();
    void onBackToHome();

private:
    QStackedWidget *stackedWidget;
    HomeScreen *homeScreen;
    GameScreen *gameScreen;
};

#endif // MAINWINDOW_H
