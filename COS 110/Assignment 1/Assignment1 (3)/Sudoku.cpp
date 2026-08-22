#include "Cell.h"
#include "Exceptions.h"
#include "Sudoku.h"


int Sudoku::onlyOnePossibilityCount = 0;
int Sudoku::onlyPlaceCount = 0;
int Sudoku::guessDigitCount = 0;
int Sudoku::solveCount = 0;

Sudoku::Sudoku(int numRows, int numCols) {
    n = numRows * numCols;
    if (n > 15) {
        throw SudokuTooBig();
    }
    this->numCols = numCols;
    this->numRows = numRows;

    board = new Cell**[n];

    for (int row = 0; row < n; row++) {
        board[row] = new Cell*[n]; 
        for (int col = 0; col < n; col++) {
           int* possibilities = new int[n];
            for (int i = 0; i < n; i++) {
                possibilities[i] = i + 1;
            }
            board[row][col] = new Cell(possibilities, n);
        }     
    }
}

Sudoku::Sudoku(string fileName) {
    ifstream file(fileName.c_str());
    file >> numRows >> numCols;
    n = numRows * numCols;

    if (n > 15) {
        throw SudokuTooBig();
    }

    board = new Cell**[n];
    for (int row = 0; row < n; row++) {
        board[row] = new Cell*[n];
        for (int col = 0; col < n; col++) {
            int* possibilities = new int[n];
            for (int i = 0; i < n; i++) {
                possibilities[i] = i + 1;
            }
            board[row][col] = new Cell(possibilities, n);
        }
    }

    string line;
    getline(file, line);
    try {
        for (int row = 0; row < n; row++) {
            getline(file, line);
            stringstream ss(line);
            string token;
            for (int col = 0; col < n; col++) {
                ss >> token;
                if (token != "-") {
                    int value;
                    stringstream(token) >> value;
                    placeDigit(row, col, value);
                }
            }
        }
    } catch (IllegalCellValue& e) {
        for (int row = 0; row < n; row++) {
            for (int col = 0; col < n; col++) {
                delete board[row][col];
            }
            delete[] board[row];
        }
    delete[] board;
    throw e;
    }       
}

Sudoku::Sudoku(const Sudoku& other) {
    numRows = other.numRows;
    numCols = other.numCols;
    n = other.n;

    board = new Cell**[n];
    for (int row = 0; row < n; row++) {
        board[row] = new Cell*[n];
        for (int col = 0; col < n; col++) {
            board[row][col] = new Cell(*other.board[row][col]);
        }
    }
}

void Sudoku::placeDigit(int row, int col, int value) {
    board[row][col]->setValue(value);

    for (int c = 0; c < n; c++) {
        Cell* cell = board[row][c];
        if (!cell->isFilledIn()) {
            for (int i = 0; i < cell->getNumPossibilities(); i++) {
                if (cell->getPossibility(i) == value) {
                    cell->removePossibility(value);
                    break;
                }
            }
        }
    }
    for (int r = 0; r < n; r++) {
        Cell* cell = board[r][col];
        if (!cell->isFilledIn()) {
            for (int i = 0; i < cell->getNumPossibilities(); i++) {
                if (cell->getPossibility(i) == value) {
                    cell->removePossibility(value);
                    break;
                }
            }
        }
    }
    int blockRowStart = (row / numRows) * numRows;
    int blockColStart = (col / numCols) * numCols;
    for (int r = blockRowStart; r < blockRowStart + numRows; r++) {
        for (int c = blockColStart; c < blockColStart + numCols; c++) {
            Cell* cell = board[r][c];
            if (!cell->isFilledIn()) {
                for (int i = 0; i < cell->getNumPossibilities(); i++) {
                    if (cell->getPossibility(i) == value) {
                        cell->removePossibility(value);
                        break;
                    }
                }
            }
        }
    }
}

bool Sudoku::isSolved() {
    bool allFilled = true;
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            Cell* cell = board[row][col];
            if (!cell->isFilledIn()) {
                allFilled = false;
                if (cell->getNumPossibilities() == 0) {
                    throw UnsolvableSudoku();
                }
            }
        }
    }
    return allFilled;
}

bool Sudoku::onlyOnePossibility(bool verbose) {
    onlyOnePossibilityCount++;

    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            Cell* cell = board[row][col];
            if (cell->getNumPossibilities() == 1) {
                int value = cell->getPossibility(0);
                placeDigit(row, col, value);
                if (verbose) {
                    cout << "onlyOnePossibility placed " << value
                         << " at r" << row << "c" << col << endl;
                }
                return true;
            }
        }
    }
    return false;
}

bool Sudoku::onlyPlace(bool verbose) {
    onlyPlaceCount++;

    for (int row = 0; row < n; row++) {
        for (int value = 1; value <= n; value++) {
            int count = 0;
            int foundCol = -1;
            for (int col = 0; col < n; col++) {
                Cell* cell = board[row][col];
                if (!cell->isFilledIn() && cell->isPossible(value)) {
                    count++;
                    foundCol = col;
                    if (count > 1) break;
                }
            }
            if (count == 1) {
                placeDigit(row, foundCol, value);
                if (verbose) {
                    cout << "onlyPlace placed " << value
                         << " at r" << row << "c" << foundCol << endl;
                }
                return true;
            }
        }
    }

    for (int col = 0; col < n; col++) {
        for (int value = 1; value <= n; value++) {
            int count = 0;
            int foundRow = -1;
            for (int row = 0; row < n; row++) {
                Cell* cell = board[row][col];
                if (!cell->isFilledIn() && cell->isPossible(value)) {
                    count++;
                    foundRow = row;
                    if (count > 1) break;
                }
            }
            if (count == 1) {
                placeDigit(foundRow, col, value);
                if (verbose) {
                    cout << "onlyPlace placed " << value
                         << " at r" << foundRow << "c" << col << endl;
                }
                return true;
            }
        }
    }

    for (int blockRow = 0; blockRow < n; blockRow += numRows) {
        for (int blockCol = 0; blockCol < n; blockCol += numCols) {
            for (int value = 1; value <= n; value++) {
                int count = 0;
                int foundRow = -1, foundCol = -1;
                for (int r = blockRow; r < blockRow + numRows; r++) {
                    for (int c = blockCol; c < blockCol + numCols; c++) {
                        Cell* cell = board[r][c];
                        if (!cell->isFilledIn() && cell->isPossible(value)) {
                            count++;
                            foundRow = r;
                            foundCol = c;
                        }
                    }
                }
                if (count == 1) {
                    placeDigit(foundRow, foundCol, value);
                    if (verbose) {
                        cout << "onlyPlace placed " << value
                             << " at r" << foundRow << "c" << foundCol << endl;
                    }
                    return true;
                }
            }
        }
    }

    return false;
}

void Sudoku::guessDigit(bool verbose) {
    guessDigitCount++;

    int guessRow = -1, guessCol = -1;
    int minPossibilities = n + 1;
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            Cell* cell = board[row][col];
            if (!cell->isFilledIn() && cell->getNumPossibilities() < minPossibilities) {
                minPossibilities = cell->getNumPossibilities();
                guessRow = row;
                guessCol = col;
            }
        }
    }

    int numPossibilities = board[guessRow][guessCol]->getNumPossibilities();
    for (int i = 0; i < numPossibilities; i++) {
        int value = board[guessRow][guessCol]->getPossibility(i);

        if (verbose) {
            cout << "guessDigit trying " << value
                 << " at r" << guessRow << "c" << guessCol << endl;
        }

        Sudoku copy(*this);
        copy.placeDigit(guessRow, guessCol, value);

        try {
            copy.solve(verbose);

            for (int row = 0; row < n; row++) {
                for (int col = 0; col < n; col++) {
                    delete board[row][col];
                    board[row][col] = new Cell(*copy.board[row][col]);
                }
            }

            if (verbose) {
                cout << "guessDigit found solution and is backtracking" << endl;
            }
            return;
        } catch (UnsolvableSudoku& e) {
            continue;
        }
    }

    throw UnsolvableSudoku();
}

void Sudoku::resetFunctionCounts() {
    onlyOnePossibilityCount = 0;
    onlyPlaceCount = 0;
    guessDigitCount = 0;
    solveCount = 0;
}

void Sudoku::printFunctionCounts() {
    cout << "Function call counts" << endl;
    cout << "onlyOnePossibility: " << onlyOnePossibilityCount << endl;
    cout << "onlyPlace: " << onlyPlaceCount << endl;
    cout << "numGuessed: " << guessDigitCount << endl;
    cout << "solve: " << solveCount << endl;
}

void Sudoku::solve(bool verbose) {
    solveCount++;

    bool solved = false;
    bool errorOccurred = false;

    while (true) {
        try {
            solved = isSolved();
        } catch (UnsolvableSudoku& e) {
            errorOccurred = true;
        }

        if (solved || errorOccurred) {
            break;
        }

        if (onlyOnePossibility(verbose)) {
            continue;
        }

        if (onlyPlace(verbose)) {
            continue;
        }

        try {
            guessDigit(verbose);
        } catch (UnsolvableSudoku& e) {
            errorOccurred = true;
        }
        break;
    }

    if (errorOccurred) {
        throw UnsolvableSudoku();
    }
}

void Sudoku::solveAndPrint(bool verbose) {
    resetFunctionCounts();
    solve(verbose);

    if (verbose) {
        cout << toStringVerbose();
    } else {
        cout << toString();
    }

    printFunctionCounts();
}

Sudoku::~Sudoku() {
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            delete board[row][col];
        }
        delete[] board[row];
    }
    delete[] board;
}