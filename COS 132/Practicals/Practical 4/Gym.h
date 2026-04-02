#ifndef GYM_H
#define GYM_H

#include <string>
#include "HealthMetrics.h"
#include "Meal.h"
#include "Workout.h"

using namespace std;

bool isGoalValid(char goal);

double kiloJouleTarget(double tdeeValue, char goal);
double proteinTarget(double weightKg, char goal);
int nutritionScore(int age, double weightKg, double heightM, const string m1, const string m2, const string m3, int activityLevel, char goal);

string goalAdvice(int age, double weightKg, double heightM, const string m1, const string m2, const string m3, int activityLevel, char goal, double prevWeight, double currWeight, int prevReps, int currReps);

#endif