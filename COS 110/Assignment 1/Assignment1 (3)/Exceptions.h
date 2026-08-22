#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class CellNotFilledIn {
   public:
    string what();
};

class CellAlreadyFilledIn {
   public:
    string what();
};

class IllegalCellValue {
   private:
    int value;

   public:
    IllegalCellValue(int value);
    string what();
};

class IllegalIndex {
   public:
    string what();
};

class SudokuTooBig {
   public:
    string what();
};

class UnsolvableSudoku {
   public:
    string what();
};

#endif