#ifndef PERMUTATIONS_H
#define PERMUTATIONS_H

#include "BasicMath.h"
#include <string>
using namespace std;

struct Statistics {
    int totalCombinations;
    int maxDepth;
    int allPrefixes;
};

struct NegativeKError {
    const char* message;
};

struct EmptySetError {
    const char* message;
};

struct NullPointerError {
    const char* message;
};

void validateInput(char set[], int k, int n);
string printAllKLength(char set[], int k, int n);

#endif // PERMUTATIONS_H