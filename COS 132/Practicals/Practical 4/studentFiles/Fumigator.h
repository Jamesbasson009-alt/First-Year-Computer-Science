#ifndef FUMIGATOR_H
#define FUMIGATOR_H
#include <string>
double calculateSolution(int effectiveArea, int bugDensity, bool severe);
int estimateTreatmentTime(int accessibleRooms, bool rushJob);
bool startFumigation(std::string bugType, int count, int houseSize, bool rushJob);


#endif