#include "Permutations.h"

using namespace std;

void validateInput(char set[], int k, int n) {

    if (set == NULL) {
        NullPointerError e;
        e.message = "Caught exception: Set pointer is NULL\n";
        throw e;
    }
    if (n <= 0) {
        EmptySetError e;
        e.message = "Caught exception: Set cannot be empty\n";
        throw e;
    }
    if (k < 0) {
        NegativeKError e;
        e.message = "Caught exception: k must be non-negative\n";
        throw e;
    }

}

string printAllKLength(char set[], int k, int n) {
    string result;
    Statistics s;
    s.allPrefixes = 0;
    s.maxDepth = 0;
    s.totalCombinations = 0;

    try {
        validateInput(set, k, n);
        
    }

}

static string intToString(int num) {
    stringstream ss;
    ss << num;
    return ss.str();
}