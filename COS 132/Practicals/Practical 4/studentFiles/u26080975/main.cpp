#include <iostream>
#include <string>
#include <cmath>

#include "Bug.h"
#include "House.h"
#include "Fumigator.h"

int main() {

    int houseSize = 670;
    int numBugsSeen = 4;
    bool rushJob = false;

    bool success = startFumigation("ant", numBugsSeen, houseSize, rushJob);
    std::cout << "Fumigation success: " << (success ? "true" : "false") << std::endl;
    //success should be true
    
    return 0;
}
