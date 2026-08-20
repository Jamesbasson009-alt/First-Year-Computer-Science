#ifndef STATSTRACKER_H
#define STATSTRACKER_H

#include <string>

class StatsTracker {
private:
    double* data;
    int size;

public:
    // Constructor: allocate space for n doubles.
    // Should throw if n <= 0.
    StatsTracker(int n);

    // Destructor: clean up dynamically allocated memory.
    ~StatsTracker();

    // Set the value at a given index.
    // Should throw if index is out of bounds.
    void set(int index, double value);

    // Get the value at a given index.
    // Should throw if index is out of bounds.
    double get(int index) const;

    // Return the size of the tracker.
    int getSize() const;
};

// Recursive function (NOT iterative) that sums all values
// in the tracker from `index` to the end.
// Base case and recursive case are up to you to design.
double recursiveSum(const StatsTracker& tracker, int index);

#endif