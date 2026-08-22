#ifndef SUDOKU_H
#define SUDOKU_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Cell.h"
#include "Exceptions.h"

using namespace std;

class Sudoku {
   private:
    Cell*** board;
    int numRows;
    int numCols;
    int n;

    static int onlyOnePossibilityCount;
    static int onlyPlaceCount;
    static int guessDigitCount;
    static int solveCount;

   public:
    Sudoku(int numRows, int numCols);
    Sudoku(string fileName);
    Sudoku(const Sudoku& other);
    ~Sudoku();

    void placeDigit(int row, int col, int value);
    bool isSolved();

    bool onlyOnePossibility(bool verbose = false);
    bool onlyPlace(bool verbose = false);
    void guessDigit(bool verbose = false);

    static void resetFunctionCounts();
    static void printFunctionCounts();
    void solve(bool verbose = false);
    void solveAndPrint(bool verbose = false);

    string toString() {
        stringstream res;
        string line = "+";
        int numDigits = ((n) / 10) + 1;
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                for (int k = 0; k < numDigits + 1; k++) {
                    line += "-";
                }
            }
            line += "-+";
        }

        for (int r = 0; r < n; r++) {
            if (r % numRows == 0) {
                res << line << endl;
            }
            for (int c = 0; c < n; c++) {
                if (c % numCols == 0) {
                    if (c != 0) {
                        res << " ";
                    }
                    res << "|";
                }
                res << setw(numDigits + 1) << board[r][c]->toString();
            }
            res << " |" << endl;
        }

        res << line << endl;
        return res.str();
    }

    string toStringVerbose() {
        string** verboseCells = new string*[n];
        int cellMaxSize = 0;
        for (int r = 0; r < n; r++) {
            verboseCells[r] = new string[n];
            for (int c = 0; c < n; c++) {
                verboseCells[r][c] = board[r][c]->toStringVerbose();
                if ((int)verboseCells[r][c].length() > cellMaxSize) {
                    cellMaxSize = (int)verboseCells[r][c].length();
                }
            }
        }

        stringstream res;
        string line = "+";
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                for (int k = 0; k < cellMaxSize + 1; k++) {
                    line += "-";
                }
            }
            line += "-+";
        }

        for (int r = 0; r < n; r++) {
            if (r % numRows == 0) {
                res << line << endl;
            }
            for (int c = 0; c < n; c++) {
                if (c % numCols == 0) {
                    if (c != 0) {
                        res << " ";
                    }
                    res << "|";
                }
                res << setw(cellMaxSize + 1) << verboseCells[r][c];
            }
            res << " |" << endl;
        }

        res << line << endl;

        for (int r = 0; r < n; r++) {
            delete[] verboseCells[r];
        }
        delete[] verboseCells;

        return res.str();
    }
};

#endif