#include "StatsTracker.h"
#include <iostream>

int main() {

    int n = 0;
    std::cout << "Please enter an n value\n";
    std::cin >> n;
    StatsTracker* tracker = NULL;

    try {
        tracker = new StatsTracker(n);
        
    } catch (std::string s) {
        std::cout << s;
        return 1;
    }

    for (int i = 0; i < n; i++) {
        try {

            std::cout << "Please enter " << i << " number\n";
            double value = 0;
            std::cin >> value;
            tracker->set(i, value);

        } catch (std::string s) {
            std::cout << s;
            i--;
        }
    }

    double sum = recursiveSum(*tracker, 0);
    std::cout << "sum = " << sum << "\n";
    std::cout << "average = " << sum / (tracker->getSize());

    delete tracker;
    return 0;
}