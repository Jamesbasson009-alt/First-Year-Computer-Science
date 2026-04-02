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

bool isSetValid(int reps, double weightKg) {
    if (reps > 30 && weightKg > 200) {
        return false;
    }
    if (reps >= 1 && reps <= 50 && weightKg >= 0 && weightKg <= 300) {
        return true;
    }
    return false;
}

char repCategory(int reps) {
    if (reps > 0 && reps <= 5) {
        return 'S';
    }
    if (reps > 5 && reps < 12) {
        return 'M';
    }
    if (reps >= 12) {
        return 'E';
    }
    return 'N';
}

int scoreFromCategory(char category) {
    if (category == 'S') {
        return 30;
    }
    if (category == 'M') {
        return 20;
    }
    if (category == 'E') {
        return 10;
    }
    return 0;
}

int intensityLevel(int reps, double weightKg) {

    if (!isSetValid(reps, weightKg)) {
        return 0;
    }
    
    if (weightKg == 0) {
        return 1;
    }

    double intensity = reps + (weightKg / 3.0);
    
    if (intensity > 22) {
        return 3;
    }
    else if (intensity > 12) {
        return 2;
    }
    else {
        return 1;
    }
}

int progressionDecision(double prevWeight, double currWeight, int prevReps, int currReps) {
    if (!isSetValid(prevReps, prevWeight) || !isSetValid(currReps, currWeight)) {
        return 0; }
    if (currWeight > prevWeight) {
        return 1;
    } else if (currWeight < prevWeight) {
        return -1;
    }
    return 0;
    
}

int progressionDecision(int prevReps, int currReps) {
    if (prevReps <= 0 || currReps <= 0) {
        return 0;
    }

    int prevIntensity = intensityLevel(prevReps, 4);
    int currIntensity = intensityLevel(currReps, 4);

    if (currIntensity > prevIntensity) {
        return 1;
    } else {
        return -1;
    }
}








