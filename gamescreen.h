#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <QWidget>
#include <QTimer>
#include <QElapsedTimer>
#include <QVector>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>

QT_BEGIN_NAMESPACE
class SudokuBoard;
class SudokuSolver;
QT_END_NAMESPACE

class GameScreen : public QWidget
{
    Q_OBJECT

public:
    explicit GameScreen(QWidget *parent = nullptr);
    void startNewGame(const QString &difficulty);
    void stopTimer();

signals:
    void backToHomeClicked();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateTimerDisplay();
    void onUndoClicked();
    void onClearAllClicked();
    void onNewGameClicked();
    void onSolveClicked();
    void onSolverCellUpdated(int row, int col, int value);
    void onSolverFinished(bool solved, int timeMs);
    void onNewGameFromOverlay();
    void onBackClicked();
    void onNumberButtonClicked(int number);
    void onCellFocused(int row, int col);

private:
    QPushButton* createButton(const QString &text, const QString &bgColor, const QString &hoverColor);
    QPushButton* createNumberButton(int number);

    SudokuBoard *board;
    SudokuSolver *solver;
    QLabel *timeCaptionLabel;
    QLabel *timerLabel;

    QVector<QPushButton*> numberButtons;
    QGridLayout *keypadLayout;
    QPushButton *newGameButton;
    QPushButton *undoButton;
    QPushButton *clearAllButton;
    QPushButton *solveButton;
    QPushButton *backButton;

    QTimer *timer;
    QElapsedTimer elapsedTimer;
    int elapsedSeconds;

    QVector<QVector<int>> initialBoard;
    QString currentDifficulty;

    int selectedRow, selectedCol;

    QWidget *overlay;
    QLabel *winMessageLabel;
    QLabel *winTimeLabel;
    QPushButton *newGameOverlayButton;

    void setupUI();
    void generatePuzzle(const QString &difficulty);
    void showWinOverlay(int timeMs);
    void hideWinOverlay();
    void setButtonsEnabled(bool enabled);
    void setKeypadEnabled(bool enabled);
};

#endif // GAMESCREEN_H
