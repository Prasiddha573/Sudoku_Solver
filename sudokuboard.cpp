#include "sudokuboard.h"
#include <QKeyEvent>
#include <QRegularExpressionValidator>
#include <QFont>
#include <QFrame>
#include <QVBoxLayout>
#include <QDebug>

SudokuBoard::SudokuBoard(QWidget *parent) : QWidget(parent)
{
    // ... (same constructor as before)
    QGridLayout *mainGrid = new QGridLayout(this);
    mainGrid->setSpacing(0);
    mainGrid->setContentsMargins(0, 0, 0, 0);

    cells.resize(9, QVector<QLineEdit*>(9, nullptr));
    readOnlyCells.resize(9, QVector<bool>(9, false));

    QFrame *outerFrame = new QFrame(this);
    outerFrame->setFrameStyle(QFrame::Box);
    outerFrame->setLineWidth(3);
    outerFrame->setStyleSheet("QFrame { border: 3px solid #2c3e50; }");
    QGridLayout *outerLayout = new QGridLayout(outerFrame);
    outerLayout->setSpacing(0);
    outerLayout->setContentsMargins(0, 0, 0, 0);

    for (int blockRow = 0; blockRow < 3; ++blockRow) {
        for (int blockCol = 0; blockCol < 3; ++blockCol) {
            QFrame *block = new QFrame(outerFrame);
            block->setFrameStyle(QFrame::Box);
            block->setLineWidth(2);
            block->setStyleSheet("QFrame { border: 2px solid #4a4a4a; }");

            QGridLayout *blockLayout = new QGridLayout(block);
            blockLayout->setSpacing(0);
            blockLayout->setContentsMargins(0, 0, 0, 0);

            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    int row = blockRow * 3 + i;
                    int col = blockCol * 3 + j;

                    QLineEdit *cell = new QLineEdit(block);
                    cell->setAlignment(Qt::AlignCenter);
                    cell->setMaxLength(1);
                    cell->setFixedSize(64, 64);
                    QFont font("Arial", 24, QFont::Bold);
                    cell->setFont(font);
                    cell->setValidator(new QRegularExpressionValidator(QRegularExpression("[1-9]"), cell));
                    cell->installEventFilter(this);

                    cell->setStyleSheet(
                        "QLineEdit { border: 1px solid #d0d0d0; background-color: white; padding-bottom: 4px; }"
                        "QLineEdit:focus { border: 2px solid #3498db; }"
                        );

                    blockLayout->addWidget(cell, i, j);
                    cells[row][col] = cell;

                    connect(cell, &QLineEdit::textEdited, this, [this, row, col](const QString &newText) {
                        onCellTextEdited(newText, row, col);
                    });
                }
            }
            outerLayout->addWidget(block, blockRow, blockCol);
        }
    }

    mainGrid->addWidget(outerFrame, 0, 0);
    setStyleSheet("background-color: #f0f0f0;");
}

void SudokuBoard::setBoard(const QVector<QVector<int>> &initial, const QVector<QVector<int>> &)
{
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            int val = initial[row][col];
            QLineEdit *cell = cells[row][col];
            if (val != 0) {
                cell->setText(QString::number(val));
                cell->setReadOnly(true);
                readOnlyCells[row][col] = true;
                cell->setStyleSheet(cell->styleSheet() + " QLineEdit { color: #1a1a1a; }");
            } else {
                cell->clear();
                cell->setReadOnly(false);
                readOnlyCells[row][col] = false;
                cell->setStyleSheet(cell->styleSheet() + " QLineEdit { color: #333333; }");
            }
        }
    }
    undoStack.clear();
    oldValueMap.clear();
    checkAllConflicts();
}

void SudokuBoard::clearUserEntries()
{
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (!readOnlyCells[row][col]) {
                cells[row][col]->clear();
            }
        }
    }
    checkAllConflicts();
}

void SudokuBoard::undo()
{
    if (!undoStack.isEmpty()) {
        UndoEntry entry = undoStack.last();
        cells[entry.row][entry.col]->setText(entry.oldValue);
        undoStack.pop_back();
        checkAllConflicts();
    }
}

void SudokuBoard::setCellReadOnly(int row, int col, bool readOnly)
{
    cells[row][col]->setReadOnly(readOnly);
    readOnlyCells[row][col] = readOnly;
}

bool SudokuBoard::isCellReadOnly(int row, int col) const
{
    return readOnlyCells[row][col];
}

int SudokuBoard::getValue(int row, int col) const
{
    QString text = cells[row][col]->text();
    return text.isEmpty() ? 0 : text.toInt();
}

void SudokuBoard::setValue(int row, int col, int value, bool recordUndo)
{
    QLineEdit *cell = cells[row][col];
    if (recordUndo && !readOnlyCells[row][col]) {
        // Store old value for undo
        QString oldText = cell->text();
        if (oldText != (value >= 1 && value <= 9 ? QString::number(value) : "")) {
            UndoEntry entry;
            entry.row = row;
            entry.col = col;
            entry.oldValue = oldText;
            undoStack.push_back(entry);
        }
    }
    if (value >= 1 && value <= 9)
        cell->setText(QString::number(value));
    else
        cell->clear();
    checkAllConflicts();
}

void SudokuBoard::setAllReadOnly(const QVector<QVector<bool>> &readOnlyMap)
{
    for (int row = 0; row < 9; ++row)
        for (int col = 0; col < 9; ++col)
            setCellReadOnly(row, col, readOnlyMap[row][col]);
}

void SudokuBoard::setAllCellsEnabled(bool enabled)
{
    for (int row = 0; row < 9; ++row)
        for (int col = 0; col < 9; ++col)
            cells[row][col]->setEnabled(enabled);
}

void SudokuBoard::checkAllConflicts()
{
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            updateConflictsForCell(row, col);
        }
    }
}

void SudokuBoard::onCellTextEdited(const QString &newText, int row, int col)
{
    QLineEdit *cell = cells[row][col];
    QString oldText = oldValueMap.value(cell, QString());
    if (oldText != newText) {
        UndoEntry entry;
        entry.row = row;
        entry.col = col;
        entry.oldValue = oldText;
        undoStack.push_back(entry);
    }
    checkAllConflicts();
}

void SudokuBoard::applyConflictStyle(int row, int col, bool hasConflict)
{
    QLineEdit *cell = cells[row][col];
    QString currentStyle = cell->styleSheet();
    currentStyle.remove("border-bottom: 2px solid red;");
    if (hasConflict) {
        currentStyle += " QLineEdit { border-bottom: 2px solid red; }";
    }
    cell->setStyleSheet(currentStyle);
}

bool SudokuBoard::hasConflict(int row, int col) const
{
    int value = getValue(row, col);
    if (value == 0) return false;

    for (int c = 0; c < 9; ++c) {
        if (c != col && getValue(row, c) == value) return true;
    }
    for (int r = 0; r < 9; ++r) {
        if (r != row && getValue(r, col) == value) return true;
    }
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int r = startRow; r < startRow + 3; ++r) {
        for (int c = startCol; c < startCol + 3; ++c) {
            if ((r != row || c != col) && getValue(r, c) == value) return true;
        }
    }
    return false;
}

void SudokuBoard::updateConflictsForCell(int row, int col)
{
    bool conflict = hasConflict(row, col);
    applyConflictStyle(row, col, conflict);
}

void SudokuBoard::storeOldValueForCell(QLineEdit *cell, int row, int col)
{
    Q_UNUSED(row);
    Q_UNUSED(col);
    oldValueMap[cell] = cell->text();
}

bool SudokuBoard::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FocusIn) {
        QLineEdit *cell = qobject_cast<QLineEdit*>(obj);
        if (cell) {
            for (int r = 0; r < 9; ++r) {
                for (int c = 0; c < 9; ++c) {
                    if (cells[r][c] == cell) {
                        storeOldValueForCell(cell, r, c);
                        emit cellFocused(r, c);   // <-- new signal emission
                        break;
                    }
                }
            }
        }
    }
    else if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Backspace) {
            undo();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}
