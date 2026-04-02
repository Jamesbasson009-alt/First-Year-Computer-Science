#ifndef WORKOUT_H
#define WORKOUT_H

#include <string>

using namespace std;

bool isSetValid(int reps, double weightKg);

int intensityLevel(int reps, double weightKg=4);
char repCategory(int reps);
int scoreFromCategory(char category);

int progressionDecision(double prevWeight, double currWeight, int prevReps, int currReps);
int progressionDecision(int prevReps, int currReps);

void printSetSummary(int reps, double weightKg);

#endif