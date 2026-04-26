#ifndef SUDOKUBOARD_H
#define SUDOKUBOARD_H

#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QVector>
#include <QMap>

class SudokuBoard : public QWidget
{
    Q_OBJECT

public:
    explicit SudokuBoard(QWidget *parent = nullptr);
    void setBoard(const QVector<QVector<int>> &initial, const QVector<QVector<int>> &solution);
    void clearUserEntries();
    void undo();
    void setCellReadOnly(int row, int col, bool readOnly);
    bool isCellReadOnly(int row, int col) const;
    int getValue(int row, int col) const;
    void setValue(int row, int col, int value, bool recordUndo = false);  // added recordUndo
    void setAllReadOnly(const QVector<QVector<bool>> &readOnlyMap);
    void setAllCellsEnabled(bool enabled);
    void checkAllConflicts();

signals:
    void cellFocused(int row, int col);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onCellTextEdited(const QString &newText, int row, int col);

private:
    QVector<QVector<QLineEdit*>> cells;
    QVector<QVector<bool>> readOnlyCells;
    QMap<QLineEdit*, QString> oldValueMap;
    struct UndoEntry { int row, col; QString oldValue; };
    QVector<UndoEntry> undoStack;

    void applyConflictStyle(int row, int col, bool hasConflict);
    bool hasConflict(int row, int col) const;
    void updateConflictsForCell(int row, int col);
    void storeOldValueForCell(QLineEdit *cell, int row, int col);
};

#endif // SUDOKUBOARD_H
