#include "HealthMetrics.h"

//DO NOT CHANGE
double HealthMetrics::bmi(double weightKg, double heightM){
    double value = 0.0;

    if (weightKg > 0.0 && heightM > 0.0)
        value = weightKg / (heightM * heightM);

    return value;
}

//DO NOT CHANGE
double HealthMetrics::bmr(int age, double weightKg, double heightM){
    double value = 0;

    if (age > 0 && weightKg > 0 && heightM > 0)
        value = (10 * weightKg) + (6.25 * heightM) - (5 * age) + 5;

    return value * 4.184;
}

//DO NOT CHANGE
double HealthMetrics::tdee(double bmrValue, int activityLevel){
    double result = 0;
    double multiplier = 1.0;

    if (bmrValue > 0){
        switch (activityLevel){
            case 1: 
                multiplier = 1.2; break;
            case 2: 
                multiplier = 1.5; break;
            case 3: 
                multiplier = 1.8; break;
            default: 
                multiplier = 1.0;
        }

        result = bmrValue * multiplier;
    }

    return result;
}

int HealthMetrics::bmiCategory(double bmiValue) {
    if (bmiValue <= 0) {
        return 0;
    } else if (bmiValue < 18.5) {
        return 1;
    } else if (bmiValue < 25 || bmiValue == 18.5) {
        return 2;
    } else if (bmiValue < 30 || bmiValue == 25) {
        return 3; 
    } else if (bmiValue >= 30) {
        return 4;
    }
}
