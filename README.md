# 🧩 Sudoku Solver Game

This is a simple Sudoku Solver game built using **C++ and Qt framework**. The main goal of this project is to generate and solve Sudoku puzzles using the **backtracking algorithm**, while also providing a clean and interactive GUI for users.

---

## 🎮 What the Game Does

The game presents a standard **9x9 Sudoku grid** where players can:

- Choose different difficulty levels
- Fill in numbers manually
- Generate puzzles automatically
- Get solutions using the solver
- Reset the board anytime

The solver uses a **recursive backtracking approach** to fill the grid efficiently and correctly.

---

## 📊 Difficulty Levels

The game includes **three levels of difficulty**:

- 🟢 Easy  
- 🟡 Medium  
- 🔴 Hard  

Each level changes the number of pre-filled cells in the 9x9 grid, making the puzzle more or less challenging.

---

## 🛠️ Technologies Used

- C++
- Qt Framework (for GUI)
- Backtracking Algorithm
- Object-Oriented Programming (OOP)

---

## 🧠 How It Works

The Sudoku solver checks each empty cell and tries numbers from 1 to 9. If a number is valid according to Sudoku rules, it moves forward. If not, it backtracks and tries another possibility. This continues until the puzzle is solved.

---

## 📌 Project Purpose

This project was created to practice and understand:
- Recursion and backtracking
- Problem-solving with algorithms
- GUI development using Qt
- Structuring a medium-sized C++ project

---