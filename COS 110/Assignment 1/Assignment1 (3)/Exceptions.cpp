#include "Exceptions.h"

string CellNotFilledIn::what() {
    return "Cell is not filled in";
}

string CellAlreadyFilledIn::what() {
    return "Cell is already filled in";
}

IllegalCellValue::IllegalCellValue(int value) : value(value) {
}

string IllegalCellValue::what() {
    stringstream ss;
    ss << "Illegal cell value: " << value;
    return ss.str();
}

string IllegalIndex::what() {
    return "Illegal Index";
}

string SudokuTooBig::what() {
    return "Sudoku board is too large";
}

string UnsolvableSudoku::what() {
    return "Sudoku board has no solutions";
}