#include "gamescreen.h"
#include "sudokuboard.h"
#include "sudokusolver.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTimer>
#include <QElapsedTimer>
#include <QRandomGenerator>
#include <QApplication>
#include <QResizeEvent>

// Predefined puzzles (same as before)
static const QVector<QVector<QVector<int>>> easyPuzzles = {
    { // Puzzle 1
        {5,3,0,0,7,0,0,0,0},
        {6,0,0,1,9,5,0,0,0},
        {0,9,8,0,0,0,0,6,0},
        {8,0,0,0,6,0,0,0,3},
        {4,0,0,8,0,3,0,0,1},
        {7,0,0,0,2,0,0,0,6},
        {0,6,0,0,0,0,2,8,0},
        {0,0,0,4,1,9,0,0,5},
        {0,0,0,0,8,0,0,7,9}
    },
    { // Puzzle 2
        {0,0,0,2,0,0,0,6,3},
        {3,0,0,0,0,5,4,0,1},
        {0,0,1,0,0,3,9,8,0},
        {0,0,0,0,9,0,0,2,0},
        {0,5,0,1,0,2,0,9,0},
        {0,2,0,0,7,0,0,0,0},
        {0,8,5,3,0,0,6,0,0},
        {9,0,3,6,0,0,0,0,7},
        {6,1,0,0,0,9,0,0,0}
    },

    { // Puzzle 3
        {1,0,0,4,8,9,0,0,6},
        {7,3,0,0,0,0,0,4,0},
        {0,0,0,0,0,1,2,9,5},
        {0,0,7,1,2,0,6,0,0},
        {5,0,0,7,0,3,0,0,8},
        {0,0,6,0,9,5,7,0,0},
        {9,1,4,6,0,0,0,0,0},
        {0,2,0,0,0,0,0,3,7},
        {8,0,0,5,1,2,0,0,4}
    },

    { // Puzzle 4
        {0,2,0,6,0,8,0,0,0},
        {5,8,0,0,0,9,7,0,0},
        {0,0,0,0,4,0,0,1,0},
        {3,7,0,2,0,0,0,6,0},
        {0,0,0,0,9,0,0,0,0},
        {0,5,0,0,0,3,0,9,2},
        {0,9,0,0,1,0,0,0,0},
        {0,0,2,8,0,0,0,7,5},
        {0,0,0,9,0,4,0,3,0}
    },

    { // Puzzle 5
        {0,0,6,0,0,0,0,0,2},
        {0,0,0,3,0,5,0,0,0},
        {0,9,0,0,0,0,4,0,0},
        {8,0,0,0,0,0,0,0,0},
        {0,0,0,7,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,9},
        {0,0,1,0,0,0,0,6,0},
        {0,0,0,2,0,8,0,0,0},
        {6,0,0,0,0,0,3,0,0}
    },

    { // Puzzle 6
        {2,0,0,0,0,0,0,0,9},
        {0,0,0,6,0,0,0,0,0},
        {0,7,4,0,8,0,0,0,0},
        {0,0,0,0,0,3,0,0,2},
        {0,8,0,0,4,0,0,1,0},
        {6,0,0,5,0,0,0,0,0},
        {0,0,0,0,1,0,7,8,0},
        {0,0,0,0,0,6,0,0,0},
        {1,0,0,0,0,0,0,0,5}
    },

    { // Puzzle 7
        {0,0,0,0,6,0,4,0,0},
        {7,0,0,0,0,3,6,0,0},
        {0,0,0,0,9,1,0,8,0},
        {0,0,0,0,0,0,0,0,0},
        {0,5,0,1,8,0,0,0,3},
        {0,0,0,3,0,6,0,4,5},
        {0,4,0,2,0,0,0,6,0},
        {9,0,3,0,0,0,0,0,0},
        {0,2,0,0,0,0,1,0,0}
    },

    { // Puzzle 8
        {0,0,5,3,0,0,0,0,0},
        {8,0,0,0,0,0,0,2,0},
        {0,7,0,0,1,0,5,0,0},
        {4,0,0,0,0,5,3,0,0},
        {0,1,0,0,7,0,0,0,6},
        {0,0,3,2,0,0,0,8,0},
        {0,6,0,5,0,0,0,0,9},
        {0,0,4,0,0,0,0,3,0},
        {0,0,0,0,0,9,7,0,0}
    },

    { // Puzzle 9
        {3,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,6,0,0,0},
        {0,0,1,0,9,5,0,0,0},
        {0,0,0,5,0,0,0,0,0},
        {0,0,0,0,7,0,0,0,0},
        {0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,2,0,0},
        {0,8,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,4,0}
    },

    { // Puzzle 10
        {0,4,0,0,0,0,8,0,5},
        {0,3,0,0,0,0,0,0,0},
        {0,0,0,7,0,0,0,0,0},
        {0,2,0,0,0,0,0,6,0},
        {0,0,0,0,8,0,4,0,0},
        {0,0,0,0,1,0,0,0,0},
        {0,0,0,6,0,3,0,7,0},
        {5,0,0,2,0,0,0,0,0},
        {1,0,4,0,0,0,0,0,0}
    },

    { // Puzzle 11
        {0,0,0,0,0,0,2,0,0},
        {0,8,0,0,0,7,0,9,0},
        {6,0,2,0,0,0,5,0,0},
        {0,7,0,0,6,0,0,0,0},
        {0,0,0,9,0,1,0,0,0},
        {0,0,0,0,2,0,0,4,0},
        {0,0,5,0,0,0,6,0,3},
        {0,9,0,4,0,0,0,7,0},
        {0,0,6,0,0,0,0,0,0}
    },

    { // Puzzle 12
        {0,0,0,0,7,0,0,0,9},
        {0,4,0,0,0,0,0,0,0},
        {0,0,8,0,0,0,0,3,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,5,0,9,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,6,0,0,0,0,8,0,0},
        {0,0,0,0,0,0,0,5,0},
        {3,0,0,0,1,0,0,0,0}
    },

    { // Puzzle 13
        {8,0,0,0,0,0,0,0,0},
        {0,0,3,6,0,0,0,0,0},
        {0,7,0,0,9,0,2,0,0},
        {0,5,0,0,0,7,0,0,0},
        {0,0,0,0,4,5,7,0,0},
        {0,0,0,1,0,0,0,3,0},
        {0,0,1,0,0,0,0,6,8},
        {0,0,8,5,0,0,0,1,0},
        {0,9,0,0,0,0,4,0,0}
    },

    { // Puzzle 14
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,3,0,8,5},
        {0,0,1,0,2,0,0,0,0},
        {0,0,0,5,0,7,0,0,0},
        {0,0,4,0,0,0,1,0,0},
        {0,9,0,0,0,0,0,0,0},
        {5,0,0,0,0,0,0,7,3},
        {0,0,2,0,1,0,0,0,0},
        {0,0,0,0,4,0,0,0,9}
    },

    { // Puzzle 15
        {0,0,0,3,0,0,0,0,0},
        {0,0,0,0,0,5,0,0,0},
        {0,0,0,0,0,0,0,7,0},
        {0,0,0,0,0,0,3,0,0},
        {0,0,1,0,0,0,0,0,0},
        {0,0,0,0,6,0,0,0,0},
        {0,0,0,0,0,0,2,0,0},
        {0,0,0,0,0,0,0,0,8},
        {0,0,0,0,0,0,0,0,0}
    }
};

static const QVector<QVector<QVector<int>>> mediumPuzzles = {
    { // Puzzle 1
        {0,0,0,2,6,0,7,0,1},
        {6,8,0,0,7,0,0,9,0},
        {0,0,0,0,0,3,0,0,4},
        {3,0,0,0,0,0,0,8,0},
        {0,0,5,0,0,9,0,0,0},
        {0,7,0,0,1,0,0,0,6},
        {0,0,0,8,0,0,1,0,0},
        {0,1,0,0,3,0,0,4,0},
        {7,0,4,0,0,0,0,0,0}
    },
    { // Puzzle 2
        {0,2,0,6,0,8,0,0,0},
        {5,8,0,0,0,9,7,0,0},
        {0,0,0,0,4,0,0,1,0},
        {3,7,0,2,0,0,0,6,0},
        {0,0,0,0,9,0,0,0,0},
        {0,5,0,0,0,3,0,9,2},
        {0,9,0,0,1,0,0,0,0},
        {0,0,2,8,0,0,0,7,5},
        {0,0,0,9,0,4,0,3,0}
    },

    { // Puzzle 3
        {0,0,0,0,0,0,2,0,0},
        {0,8,0,0,0,7,0,9,0},
        {6,0,2,0,0,0,5,0,0},
        {0,7,0,0,6,0,0,0,0},
        {0,0,0,9,0,1,0,0,0},
        {0,0,0,0,2,0,0,4,0},
        {0,0,5,0,0,0,6,0,3},
        {0,9,0,4,0,0,0,7,0},
        {0,0,6,0,0,0,0,0,0}
    },

    { // Puzzle 4
        {1,0,0,0,0,7,0,9,0},
        {0,3,0,0,2,0,0,0,8},
        {0,0,9,6,0,0,5,0,0},
        {0,0,5,3,0,0,9,0,0},
        {0,1,0,0,8,0,0,0,2},
        {6,0,0,0,0,4,0,0,0},
        {3,0,0,0,0,0,0,1,0},
        {0,4,0,0,0,0,0,0,7},
        {0,0,7,0,0,0,3,0,0}
    },

    { // Puzzle 5
        {0,0,0,6,0,0,4,0,0},
        {7,0,0,0,0,3,6,0,0},
        {0,0,0,0,9,1,0,8,0},
        {0,0,0,0,0,0,0,0,0},
        {0,5,0,1,8,0,0,0,3},
        {0,0,0,3,0,6,0,4,5},
        {0,4,0,2,0,0,0,6,0},
        {9,0,3,0,0,0,0,0,0},
        {0,2,0,0,0,0,1,0,0}
    },

    { // Puzzle 6
        {0,0,0,0,0,0,0,1,2},
        {0,0,0,0,3,5,0,0,0},
        {0,0,0,7,0,0,0,0,0},
        {0,0,0,0,0,0,3,0,0},
        {0,0,1,0,0,0,0,0,0},
        {0,0,0,0,6,0,0,0,0},
        {0,0,0,0,0,0,2,0,0},
        {0,0,0,0,0,0,0,0,8},
        {0,0,0,0,0,0,0,0,0}
    },

    { // Puzzle 7
        {0,6,0,0,0,0,0,0,0},
        {0,0,0,3,0,5,0,0,0},
        {0,0,0,0,0,0,4,0,0},
        {8,0,0,0,0,0,0,0,0},
        {0,0,0,7,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,9},
        {0,0,1,0,0,0,0,6,0},
        {0,0,0,2,0,8,0,0,0},
        {6,0,0,0,0,0,3,0,0}
    },

    { // Puzzle 8
        {2,0,0,0,0,0,0,0,9},
        {0,0,0,6,0,0,0,0,0},
        {0,7,4,0,8,0,0,0,0},
        {0,0,0,0,0,3,0,0,2},
        {0,8,0,0,4,0,0,1,0},
        {6,0,0,5,0,0,0,0,0},
        {0,0,0,0,1,0,7,8,0},
        {0,0,0,0,0,6,0,0,0},
        {1,0,0,0,0,0,0,0,5}
    },

    { // Puzzle 9
        {0,0,5,3,0,0,0,0,0},
        {8,0,0,0,0,0,0,2,0},
        {0,7,0,0,1,0,5,0,0},
        {4,0,0,0,0,5,3,0,0},
        {0,1,0,0,7,0,0,0,6},
        {0,0,3,2,0,0,0,8,0},
        {0,6,0,5,0,0,0,0,9},
        {0,0,4,0,0,0,0,3,0},
        {0,0,0,0,0,9,7,0,0}
    },

    { // Puzzle 10
        {3,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,6,0,0,0},
        {0,0,1,0,9,5,0,0,0},
        {0,0,0,5,0,0,0,0,0},
        {0,0,0,0,7,0,0,0,0},
        {0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,2,0,0},
        {0,8,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,4,0}
    },

    { // Puzzle 11
        {0,4,0,0,0,0,8,0,5},
        {0,3,0,0,0,0,0,0,0},
        {0,0,0,7,0,0,0,0,0},
        {0,2,0,0,0,0,0,6,0},
        {0,0,0,0,8,0,4,0,0},
        {0,0,0,0,1,0,0,0,0},
        {0,0,0,6,0,3,0,7,0},
        {5,0,0,2,0,0,0,0,0},
        {1,0,4,0,0,0,0,0,0}
    },

    { // Puzzle 12
        {0,0,0,0,7,0,0,0,9},
        {0,4,0,0,0,0,0,0,0},
        {0,0,8,0,0,0,0,3,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,5,0,9,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,6,0,0,0,0,8,0,0},
        {0,0,0,0,0,0,0,5,0},
        {3,0,0,0,1,0,0,0,0}
    },

    { // Puzzle 13
        {8,0,0,0,0,0,0,0,0},
        {0,0,3,6,0,0,0,0,0},
        {0,7,0,0,9,0,2,0,0},
        {0,5,0,0,0,7,0,0,0},
        {0,0,0,0,4,5,7,0,0},
        {0,0,0,1,0,0,0,3,0},
        {0,0,1,0,0,0,0,6,8},
        {0,0,8,5,0,0,0,1,0},
        {0,9,0,0,0,0,4,0,0}
    },

    { // Puzzle 14
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,3,0,8,5},
        {0,0,1,0,2,0,0,0,0},
        {0,0,0,5,0,7,0,0,0},
        {0,0,4,0,0,0,1,0,0},
        {0,9,0,0,0,0,0,0,0},
        {5,0,0,0,0,0,0,7,3},
        {0,0,2,0,1,0,0,0,0},
        {0,0,0,0,4,0,0,0,9}
    },

    { // Puzzle 15
        {0,0,0,3,0,0,0,0,0},
        {0,0,0,0,0,5,0,0,0},
        {0,0,0,0,0,0,0,7,0},
        {0,0,0,0,0,0,3,0,0},
        {0,0,1,0,0,0,0,0,0},
        {0,0,0,0,6,0,0,0,0},
        {0,0,0,0,0,0,2,0,0},
        {0,0,0,0,0,0,0,0,8},
        {0,0,0,0,0,0,0,0,0}
    }
};

static const QVector<QVector<QVector<int>>> hardPuzzles = {
    { // Puzzle 1
        {0,0,0,6,0,0,4,0,0},
        {7,0,0,0,0,3,6,0,0},
        {0,0,0,0,9,1,0,8,0},
        {0,0,0,0,0,0,0,0,0},
        {0,5,0,1,8,0,0,0,3},
        {0,0,0,3,0,6,0,4,5},
        {0,4,0,2,0,0,0,6,0},
        {9,0,3,0,0,0,0,0,0},
        {0,2,0,0,0,0,1,0,0}
    },
    { // Puzzle 2
        {0,8,0,0,0,9,0,3,0},
        {6,0,0,7,0,0,1,0,0},
        {0,2,0,0,4,0,0,0,9},
        {2,0,9,0,0,3,0,0,7},
        {0,0,0,0,1,0,0,0,0},
        {8,0,0,6,0,0,2,0,3},
        {4,0,0,0,9,0,0,7,0},
        {0,0,7,0,0,8,0,0,5},
        {0,3,0,5,0,0,0,1,0}
    },

    { // Puzzle 3
        {0,0,0,0,0,0,0,1,2},
        {0,0,0,0,3,5,0,0,0},
        {0,0,0,7,0,0,0,0,0},
        {0,0,0,0,0,0,3,0,0},
        {0,0,1,0,0,0,0,0,0},
        {0,0,0,0,6,0,0,0,0},
        {0,0,0,0,0,0,2,0,0},
        {0,0,0,0,0,0,0,0,8},
        {0,0,0,0,0,0,0,0,0}
    },

    { // Puzzle 4
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,3,0,8,5},
        {0,0,1,0,2,0,0,0,0},
        {0,0,0,5,0,7,0,0,0},
        {0,0,4,0,0,0,1,0,0},
        {0,9,0,0,0,0,0,0,0},
        {5,0,0,0,0,0,0,7,3},
        {0,0,2,0,1,0,0,0,0},
        {0,0,0,0,4,0,0,0,9}
    },

    { // Puzzle 5
        {8,0,0,0,0,0,0,0,0},
        {0,0,3,6,0,0,0,0,0},
        {0,7,0,0,9,0,2,0,0},
        {0,5,0,0,0,7,0,0,0},
        {0,0,0,0,4,5,7,0,0},
        {0,0,0,1,0,0,0,3,0},
        {0,0,1,0,0,0,0,6,8},
        {0,0,8,5,0,0,0,1,0},
        {0,9,0,0,0,0,4,0,0}
    },

    { // Puzzle 6
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0}
    },

    { // Puzzle 7
        {0,0,0,0,0,0,0,0,1},
        {0,0,0,0,0,2,0,0,0},
        {0,0,0,0,3,0,0,0,0},
        {0,0,0,4,0,0,0,0,0},
        {0,0,5,0,0,0,0,0,0},
        {0,6,0,0,0,0,0,0,0},
        {7,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,8,0,0},
        {0,0,0,0,0,0,0,0,0}
    },

    { // Puzzle 8
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0}
    },

    { // Puzzle 9
        {1,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,2},
        {0,0,0,0,0,3,0,0,0},
        {0,0,0,0,4,0,0,0,0},
        {0,0,0,5,0,0,0,0,0},
        {0,0,6,0,0,0,0,0,0},
        {0,7,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,8,0,0},
        {0,0,0,0,0,0,0,0,0}
    },

    { // Puzzle 10
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,9,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0}
    },

    { // Puzzle 11
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,2,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0}
    },

    { // Puzzle 12
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,3,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0}
    },

    { // Puzzle 13
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,4,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0}
    },

    { // Puzzle 14
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,5,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0}
    },

    { // Puzzle 15
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,6,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0}
    }
};

GameScreen::GameScreen(QWidget *parent) : QWidget(parent)
{
    selectedRow = selectedCol = -1;
    setupUI();
    solver = new SudokuSolver(this);
    connect(solver, &SudokuSolver::cellUpdated, this, &GameScreen::onSolverCellUpdated);
    connect(solver, &SudokuSolver::solvingFinished, this, &GameScreen::onSolverFinished);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameScreen::updateTimerDisplay);
    elapsedSeconds = 0;
}

QPushButton* GameScreen::createButton(const QString &text, const QString &bgColor, const QString &hoverColor)
{
    QPushButton *btn = new QPushButton(text, this);
    btn->setFixedSize(230, 45);  // now uniform width with keypad
    btn->setFont(QFont("Arial", 11, QFont::Bold));
    btn->setStyleSheet(QString(
                           "QPushButton { background-color: %1; color: white; border: none; border-radius: 6px; }"
                           "QPushButton:hover { background-color: %2; }"
                           ).arg(bgColor, hoverColor));
    return btn;
}

QPushButton* GameScreen::createNumberButton(int number)
{
    QPushButton *btn = new QPushButton(QString::number(number), this);
    btn->setFixedSize(70, 70);
    btn->setFont(QFont("Arial", 24, QFont::Bold));
    btn->setStyleSheet(
        "QPushButton { background-color: #ecf0f1; color: #2c3e50; border: 1px solid #bdc3c7; border-radius: 10px; }"
        "QPushButton:hover { background-color: #bdc3c7; }"
        "QPushButton:pressed { background-color: #95a5a6; }"
        );
    connect(btn, &QPushButton::clicked, this, [this, number]() { onNumberButtonClicked(number); });
    return btn;
}

void GameScreen::setupUI()
{
    // Main horizontal layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(80, 20, 20, 20);
    mainLayout->setSpacing(30);

    // Left: Sudoku board
    board = new SudokuBoard(this);
    board->setFixedSize(600, 600);
    connect(board, &SudokuBoard::cellFocused, this, &GameScreen::onCellFocused);
    mainLayout->addWidget(board);

    // Right panel: vertical layout
    QVBoxLayout *rightPanel = new QVBoxLayout();
    rightPanel->setSpacing(15);
    rightPanel->setAlignment(Qt::AlignTop);

    // Timer section
    timeCaptionLabel = new QLabel("Time", this);
    timeCaptionLabel->setFont(QFont("Arial", 12));
    timeCaptionLabel->setAlignment(Qt::AlignCenter);
    timeCaptionLabel->setStyleSheet("color: #7f8c8d;");

    timerLabel = new QLabel("00:00", this);
    QFont timerFont("Arial", 20, QFont::Bold);
    timerLabel->setFont(timerFont);
    timerLabel->setStyleSheet(
        "QLabel { color: #2c3e50; background-color: white; border-radius: 6px; padding: 5px; }"
        );
    timerLabel->setAlignment(Qt::AlignCenter);
    timerLabel->setFixedWidth(120);

    rightPanel->addWidget(timeCaptionLabel);
    rightPanel->addWidget(timerLabel, 0, Qt::AlignHCenter);
    rightPanel->addSpacing(10);

    // Keypad 3x3 grid
    keypadLayout = new QGridLayout();
    keypadLayout->setSpacing(10);
    int pos = 0;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            int num = pos + 1;
            QPushButton *btn = createNumberButton(num);
            numberButtons.append(btn);
            keypadLayout->addWidget(btn, r, c);
            pos++;
        }
    }
    rightPanel->addLayout(keypadLayout);

    // New Game button (already 230px)
    newGameButton = new QPushButton("NEW GAME", this);
    newGameButton->setFixedSize(230, 50);
    newGameButton->setFont(QFont("Arial", 14, QFont::Bold));
    newGameButton->setStyleSheet(
        "QPushButton { background-color: #2ecc71; color: white; border: none; border-radius: 8px; }"
        "QPushButton:hover { background-color: #27ae60; }"
        );
    connect(newGameButton, &QPushButton::clicked, this, &GameScreen::onNewGameClicked);
    rightPanel->addWidget(newGameButton, 0, Qt::AlignHCenter);

    rightPanel->addSpacing(5);

    // Other three buttons (now same width)
    undoButton = createButton("UNDO", "#3498db", "#2980b9");
    clearAllButton = createButton("CLEAR ALL", "#e67e22", "#d35400");
    solveButton = createButton("SOLVE", "#9b59b6", "#8e44ad");

    connect(undoButton, &QPushButton::clicked, this, &GameScreen::onUndoClicked);
    connect(clearAllButton, &QPushButton::clicked, this, &GameScreen::onClearAllClicked);
    connect(solveButton, &QPushButton::clicked, this, &GameScreen::onSolveClicked);

    rightPanel->addWidget(undoButton);
    rightPanel->addWidget(clearAllButton);
    rightPanel->addWidget(solveButton);
    rightPanel->addStretch();

    mainLayout->addLayout(rightPanel);

    // Top-left back button (arrow)
    backButton = new QPushButton("←", this);
    backButton->setFont(QFont("Arial", 24));
    backButton->setFixedSize(40, 40);
    backButton->setStyleSheet(
        "QPushButton { background-color: transparent; color: #2c3e50; border: none; }"
        "QPushButton:hover { color: #3498db; }"
        );
    backButton->move(10, 10);
    backButton->raise();
    connect(backButton, &QPushButton::clicked, this, &GameScreen::onBackClicked);

    // Win overlay (increased height)
    overlay = new QWidget(this);
    overlay->setGeometry(rect());
    overlay->setStyleSheet("background-color: rgba(0, 0, 0, 180);");
    overlay->hide();

    QVBoxLayout *overlayLayout = new QVBoxLayout(overlay);
    overlayLayout->setAlignment(Qt::AlignCenter);

    QWidget *centerBox = new QWidget(overlay);
    centerBox->setStyleSheet("background-color: white; border-radius: 20px; padding: 30px;");
    centerBox->setFixedSize(450, 350);  // increased height
    QVBoxLayout *centerLayout = new QVBoxLayout(centerBox);
    centerLayout->setAlignment(Qt::AlignCenter);

    winMessageLabel = new QLabel("🎉 You Win! 🎉", centerBox);
    winMessageLabel->setFont(QFont("Arial", 28, QFont::Bold));
    winMessageLabel->setAlignment(Qt::AlignCenter);
    winMessageLabel->setStyleSheet("color: #27ae60;");
    centerLayout->addWidget(winMessageLabel);

    winTimeLabel = new QLabel(centerBox);
    winTimeLabel->setFont(QFont("Arial", 20));
    winTimeLabel->setAlignment(Qt::AlignCenter);
    winTimeLabel->setStyleSheet("color: #2c3e50; margin: 20px;");
    winTimeLabel->setFixedHeight(50);  // ensure full visibility
    centerLayout->addWidget(winTimeLabel);

    newGameOverlayButton = new QPushButton("New Game", centerBox);
    newGameOverlayButton->setFont(QFont("Arial", 16));
    newGameOverlayButton->setStyleSheet(
        "QPushButton { background-color: #3498db; color: white; border-radius: 10px; padding: 12px 30px; }"
        "QPushButton:hover { background-color: #2980b9; }"
        );
    connect(newGameOverlayButton, &QPushButton::clicked, this, &GameScreen::onNewGameFromOverlay);
    centerLayout->addWidget(newGameOverlayButton);

    overlayLayout->addWidget(centerBox);
}

void GameScreen::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    overlay->setGeometry(rect());
}

void GameScreen::onCellFocused(int row, int col)
{
    selectedRow = row;
    selectedCol = col;
}

void GameScreen::onNumberButtonClicked(int number)
{
    if (selectedRow == -1 || selectedCol == -1) return;
    if (board->isCellReadOnly(selectedRow, selectedCol)) return;
    board->setValue(selectedRow, selectedCol, number, true);  // record undo
}

void GameScreen::onBackClicked()
{
    stopTimer();
    emit backToHomeClicked();
}

void GameScreen::startNewGame(const QString &difficulty)
{
    currentDifficulty = difficulty;
    generatePuzzle(difficulty);

    elapsedSeconds = 0;
    timerLabel->setText("00:00");
    elapsedTimer.start();
    timer->start(1000);

    hideWinOverlay();
    setButtonsEnabled(true);
    setKeypadEnabled(true);
    board->setAllCellsEnabled(true);
    selectedRow = selectedCol = -1;
}

void GameScreen::stopTimer()
{
    timer->stop();
}

void GameScreen::generatePuzzle(const QString &difficulty)
{
    int index;
    if (difficulty == "easy") {
        index = QRandomGenerator::global()->bounded(easyPuzzles.size());
        initialBoard = easyPuzzles[index];
    } else if (difficulty == "medium") {
        index = QRandomGenerator::global()->bounded(mediumPuzzles.size());
        initialBoard = mediumPuzzles[index];
    } else {
        index = QRandomGenerator::global()->bounded(hardPuzzles.size());
        initialBoard = hardPuzzles[index];
    }
    board->setBoard(initialBoard, initialBoard);
}

void GameScreen::updateTimerDisplay()
{
    elapsedSeconds = elapsedTimer.elapsed() / 1000;
    int minutes = elapsedSeconds / 60;
    int seconds = elapsedSeconds % 60;
    timerLabel->setText(QString("%1:%2")
                            .arg(minutes, 2, 10, QChar('0'))
                            .arg(seconds, 2, 10, QChar('0')));
}

void GameScreen::onUndoClicked()
{
    board->undo();
}

void GameScreen::onClearAllClicked()
{
    board->clearUserEntries();
}

void GameScreen::onNewGameClicked()
{
    generatePuzzle(currentDifficulty);
    elapsedSeconds = 0;
    timerLabel->setText("00:00");
    elapsedTimer.restart();
}

void GameScreen::onSolveClicked()
{
    board->setAllCellsEnabled(false);
    setButtonsEnabled(false);
    setKeypadEnabled(false);

    QVector<QVector<int>> currentBoard(9, QVector<int>(9, 0));
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            currentBoard[r][c] = board->getValue(r, c);

    solver->setBoard(currentBoard);
    solver->startSolving(50);
}

void GameScreen::onSolverCellUpdated(int row, int col, int value)
{
    board->setValue(row, col, value, false);  // no undo for solver
    QApplication::processEvents();
}

void GameScreen::onSolverFinished(bool solved, int timeMs)
{
    if (solved) {
        timer->stop();
        showWinOverlay(timeMs);
    } else {
        board->setAllCellsEnabled(true);
        setButtonsEnabled(true);
        setKeypadEnabled(true);
    }
}

void GameScreen::onNewGameFromOverlay()
{
    hideWinOverlay();
    startNewGame(currentDifficulty);
}

void GameScreen::showWinOverlay(int timeMs)
{
    int seconds = timeMs / 1000;
    int minutes = seconds / 60;
    seconds %= 60;
    winTimeLabel->setText(QString("Completed in %1:%2")
                              .arg(minutes, 2, 10, QChar('0'))
                              .arg(seconds, 2, 10, QChar('0')));
    overlay->setGeometry(rect());
    overlay->show();
    overlay->raise();
}

void GameScreen::hideWinOverlay()
{
    overlay->hide();
}

void GameScreen::setButtonsEnabled(bool enabled)
{
    undoButton->setEnabled(enabled);
    clearAllButton->setEnabled(enabled);
    solveButton->setEnabled(enabled);
    backButton->setEnabled(enabled);
    newGameButton->setEnabled(enabled);
}

void GameScreen::setKeypadEnabled(bool enabled)
{
    // Fix clazy warning: use qAsConst
    for (QPushButton *btn : qAsConst(numberButtons))
        btn->setEnabled(enabled);
}
