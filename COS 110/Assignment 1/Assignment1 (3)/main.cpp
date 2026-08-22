#include "Cell.h"
#include "Exceptions.h"
#include "Sudoku.h"

int main() {
    /*
        Output is given in output.txt
    */
    Sudoku easySudoku("easySudoku.txt");
    cout << easySudoku.toStringVerbose();
    easySudoku.solveAndPrint(true);
    Sudoku emptySudoku(3, 3);
    cout << emptySudoku.toStringVerbose();
    emptySudoku.solveAndPrint();
    return 0;
}