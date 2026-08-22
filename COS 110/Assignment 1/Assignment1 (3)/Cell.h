#ifndef CELL_H
#define CELL_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Exceptions.h"

using namespace std;

class Cell {
   private:
    int* possibilities;
    int numPossibilities;
    int* value;
    bool filledIn;

   public:
    Cell(int value);
    Cell(int* possibilities, int numPossibilities);
    Cell(const Cell& other);
    ~Cell();

    bool isFilledIn();
    void setValue(int value);
    int getValue();
    int getNumPossibilities();
    int getPossibility(int index);
    bool isPossible(int value);
    void removePossibility(int value);

    string toString() {
        if (filledIn) {
            stringstream ss;
            ss << *value;
            return ss.str();
        } else {
            return "-";
        }
    }

    string toStringVerbose() {
        stringstream ss;
        if (filledIn) {
            ss << *value;
        } else {
            ss << "[";
            for (int i = 0; i < numPossibilities; i++) {
                if (i != 0) {
                    ss << ",";
                }
                ss << possibilities[i];
            }
            ss << "]";
        }
        return ss.str();
    }
};

#endif