#include "sudokusolver.h"
#include <QDebug>

SudokuSolver::SudokuSolver(QObject *parent) : QObject(parent)
{
    stepTimer = new QTimer(this);
    connect(stepTimer, &QTimer::timeout, this, &SudokuSolver::performStep);
    solving = false;
    delay = 50;
}

void SudokuSolver::setBoard(const QVector<QVector<int>> &board)
{
    this->board = board;
}

void SudokuSolver::startSolving(int delayMs)
{
    if (solving) return;
    delay = delayMs;
    solving = true;
    backtracking = false;
    stack.clear();
    currentRow = 0;
    currentCol = 0;
    nextNum = 1;
    elapsedTimer.start();
    startIterativeSolver();
    stepTimer->start(delay);
}

void SudokuSolver::stopSolving()
{
    if (solving) {
        stepTimer->stop();
        solving = false;
    }
}

bool SudokuSolver::isSolving() const
{
    return solving;
}

void SudokuSolver::performStep()
{
    if (!solving) return;

    // Find next empty cell
    if (!findNextEmpty(currentRow, currentCol)) {
        // No empty cells – puzzle solved
        stepTimer->stop();
        solving = false;
        emit solvingFinished(true, elapsedTimer.elapsed());
        return;
    }

    // Try numbers from nextNum (or 1 if we are at a new cell)
    if (!backtracking) {
        nextNum = 1;
    }

    bool placed = false;
    for (int num = nextNum; num <= 9; ++num) {
        if (isValid(currentRow, currentCol, num)) {
            // Place the number
            board[currentRow][currentCol] = num;
            emit cellUpdated(currentRow, currentCol, num);
            // Push step onto stack
            Step step;
            step.row = currentRow;
            step.col = currentCol;
            step.currentNum = num;
            step.forward = true;
            stack.push_back(step);
            // Move to next cell
            backtracking = false;
            placed = true;
            break;
        }
    }

    if (!placed) {
        // No valid number – need to backtrack
        if (stack.isEmpty()) {
            // No solution
            stepTimer->stop();
            solving = false;
            emit solvingFinished(false, elapsedTimer.elapsed());
            return;
        }

        // Backtrack: pop last step, clear that cell, and set nextNum to that number+1
        Step last = stack.back();
        stack.pop_back();
        board[last.row][last.col] = 0;
        emit cellUpdated(last.row, last.col, 0);
        currentRow = last.row;
        currentCol = last.col;
        nextNum = last.currentNum + 1;
        backtracking = true;
    } else {
        // Successfully placed, next step will find next empty cell
        backtracking = false;
    }
}

bool SudokuSolver::findNextEmpty(int &row, int &col)
{
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (board[r][c] == 0) {
                row = r;
                col = c;
                return true;
            }
        }
    }
    return false;
}

bool SudokuSolver::isValid(int row, int col, int num)
{
    // Check row
    for (int c = 0; c < 9; ++c) {
        if (board[row][c] == num) return false;
    }
    // Check column
    for (int r = 0; r < 9; ++r) {
        if (board[r][col] == num) return false;
    }
    // Check 3x3 box
    int boxRow = row / 3 * 3;
    int boxCol = col / 3 * 3;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (board[boxRow + r][boxCol + c] == num) return false;
        }
    }
    return true;
}

void SudokuSolver::startIterativeSolver()
{
    // Initialize by finding first empty cell
    if (!findNextEmpty(currentRow, currentCol)) {
        // Already solved
        emit solvingFinished(true, 0);
        solving = false;
    }
}
