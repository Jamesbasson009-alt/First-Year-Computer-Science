#include "Cell.h"
#include "Exceptions.h"

Cell::Cell(int value) {
    possibilities = NULL;
    numPossibilities = -1;
    this->value = new int(value);
    filledIn = true;
}

Cell::Cell(int* possibilities, int numPossibilities) {
    this->possibilities = possibilities;
    this->numPossibilities = numPossibilities;
    value = NULL;
    filledIn = false;
}

Cell::Cell(const Cell& other) {
    numPossibilities = other.numPossibilities;
    filledIn = other.filledIn;
    if (other.value != NULL) {
        value = new int(*other.value);
    } else {
        value = NULL;
    }
    
    if (other.possibilities != NULL) {
        possibilities = new int[numPossibilities];
        for (int i = 0; i < numPossibilities; i++) {
            possibilities[i] = other.possibilities[i];
        }
    } else {
        possibilities = NULL;
    }
}

Cell::~Cell() {
    delete value;
    delete[] possibilities;
}

bool Cell::isFilledIn() {
    return filledIn;
}

void Cell::setValue(int value) {
    if (this->value != NULL) {
        throw CellAlreadyFilledIn();
    }
    bool found = false;
    for (int i = 0; i < numPossibilities; i++) {
        if (value == possibilities[i]) {
            found = true;
            break;
        }
    }
    if (!found) {
        throw IllegalCellValue(value);
    }
        
    this->value = new int(value);
    delete[] possibilities;
    possibilities = NULL;
    numPossibilities = -1;
    filledIn = true;
}

int Cell::getValue() {
    if (value == NULL) {
        throw CellNotFilledIn();
    }
    return *value;
        
}

int Cell::getNumPossibilities() {
    return numPossibilities;
}
    
int Cell::getPossibility(int index) {
    if (index > numPossibilities - 1 || index < 0) {
        throw IllegalIndex();
    }
    return possibilities[index];
}

bool Cell::isPossible(int value) {
    bool flag = false;
    for (int i = 0; i < numPossibilities; i++) {
        if (value == possibilities[i]) {
            flag = true;
        }
    }
    return flag;
}

void Cell::removePossibility(int value) {
    bool found = false;
    for (int i = 0; i < numPossibilities; i++) {
        if (possibilities[i] == value) {
            found = true;
            break;
        }
    }
    if (!found) {
        return;
    }

    int* newArr = new int[numPossibilities - 1];
    int write = 0;
    for (int i = 0; i < numPossibilities; i++) {
        if (possibilities[i] != value) {
            newArr[write] = possibilities[i];
            write++;
        }
    }
    numPossibilities--;
    delete[] possibilities;
    possibilities = newArr;
}