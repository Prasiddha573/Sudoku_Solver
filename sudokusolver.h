#ifndef SUDOKUSOLVER_H
#define SUDOKUSOLVER_H

#include <QObject>
#include <QVector>
#include <QTimer>
#include <QElapsedTimer>

class SudokuSolver : public QObject
{
    Q_OBJECT

public:
    explicit SudokuSolver(QObject *parent = nullptr);
    void setBoard(const QVector<QVector<int>> &board);
    void startSolving(int delayMs = 50); // delay between steps
    void stopSolving();
    bool isSolving() const;

signals:
    void cellUpdated(int row, int col, int value); // value 0 means clear
    void solvingFinished(bool solved, int timeMs);

private slots:
    void performStep();

private:
    QVector<QVector<int>> board;
    QTimer *stepTimer;
    QElapsedTimer elapsedTimer;
    bool solving;
    int delay;

    struct Step {
        int row, col;
        int currentNum;   // the number we are trying (0 means not tried yet)
        bool forward;     // true if we are moving forward, false when backtracking
    };
    QVector<Step> stack;
    int currentRow, currentCol;
    int nextNum;
    bool backtracking;

    bool findNextEmpty(int &row, int &col);
    bool isValid(int row, int col, int num);
    void startIterativeSolver();
};

#endif // SUDOKUSOLVER_H
