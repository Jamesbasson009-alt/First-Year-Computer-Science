#include "Workout.h"
#include <iostream>

//DO NOT CHANGE
void printSetSummary(int reps, double weightKg){
    cout << "Workout Set: " << reps << " reps @ " << weightKg << " kg\n";
    int intensity = intensityLevel(reps, weightKg);
    cout << "Intensity Level: " << intensity << " ";

    if (intensity == 3) 
        cout << "(Heavy)\n";
    else if (intensity == 2) 
        cout << "(Moderate)\n";
    else 
        cout << "(Light)\n";
}
