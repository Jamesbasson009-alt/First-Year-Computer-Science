#ifndef HEALTH_METRICS_H
#define HEALTH_METRICS_H

namespace HealthMetrics{
    double bmi(double weightKg, double heightM);
    double bmr(int age, double weightKg, double heightM);
    double tdee(double bmrValue, int activityLevel);
    int bmiCategory(double bmiValue);
}

#endif