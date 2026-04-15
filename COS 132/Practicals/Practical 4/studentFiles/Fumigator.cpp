#include <cmath>
#include <iostream>

#include "Fumigator.h"
#include "House.h"
#include "Bug.h"

double calculateSolution(int effectiveArea, int bugDensity, bool severe){

    double base = effectiveArea * bugDensity * 0.75;
    if(severe)
        base *= 1.5;

    return base;
}

int estimateTreatmentTime(int accessibleRooms, bool rushJob){

    int time = accessibleRooms * 30;

    if (rushJob)
        time = round(time * 0.75);

    return time;
}

bool startFumigation(std::string bugType, int count, int houseSize, bool rushJob){

    double solution = calculateSolution(getEffectiveArea(houseSize), getBugDensity(bugType), isSevereInfestation(bugType, count));

    std::cout << "Starting fumigation of " << bugType << "(s/es) which are \"" << getBugInfo(bugType) << "\" with "<< solution << "ml of solution." << std::endl;
    
    int roomsFumigated = fumigate(solution);

    int estimatedTime = estimateTreatmentTime(countAccessibleRooms(), rushJob);
    int actualTime = estimateTreatmentTime(roomsFumigated, rushJob);

    return (roomsFumigated >= 3) && (actualTime <= estimatedTime);
}