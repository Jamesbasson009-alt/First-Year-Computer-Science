#include "StatsTracker.h"

StatsTracker::StatsTracker(int n) {
    if (n <= 0) {
        throw std::string("Invalid size");
    } else {
        size = n;
        data = new double[n];
    }
}

StatsTracker::~StatsTracker() {
    delete[] data;
    data = NULL;
}
    
void StatsTracker::set(int index, double value) {
    if (index >= size || index < 0) {
        throw std::string("Index out of bounds");
    }
    data[index] = value; 
}

double StatsTracker::get(int index) const {
    return data[index];
}



int StatsTracker::getSize() const {
    return size;
}

double recursiveSum(const StatsTracker& tracker, int index) {
    if (index > tracker.getSize()) {
        return 0.0;
    }

    return tracker.get(index) + recursiveSum(tracker, index + 1);

}
    
    