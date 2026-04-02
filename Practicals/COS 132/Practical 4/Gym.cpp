#include "Gym.h"
using namespace HealthMetrics;
using namespace Constants;

//DO NOT CHANGE
string goalAdvice(int age, double weightKg, double heightM, const string m1, const string m2, const string m3, int activityLevel, char goal, double prevWeight, double currWeight, int prevReps, int currReps){
    string advice = "";

    double bmiVal  = bmi(weightKg, heightM);
    int category = bmiCategory(bmiVal);

    //BMI
    switch (category){
        case 1:
            advice += "Your BMI is below the recommended range. A gradual increase in kiloJoule and protein intake may help.\n";
            break;

        case 4:
            advice += "Your BMI is above the recommended range. A slight reduction in kiloJoule intake may be beneficial.\n";
            break;

        case 3:
            advice += "Your BMI is slightly above the recommended range. Small adjustments to diet and activity could help.\n";
            break;

        default:
            advice += "Your BMI is within a healthy range.\n";
            break;
    }

    //Nutrition
    int nScore = nutritionScore(age, weightKg, heightM, m1, m2, m3, activityLevel, goal);

    if (nScore < 4)
        advice += "Your nutrition could be improved. Consider aligning your intake with your targets.\n";
    else if (nScore < 7)
        advice += "Your nutrition is fairly balanced with some room for improvement.\n";
    else
        advice += "Your nutrition is well aligned with your goal.\n";

    //Workout progression
    int progressReps = progressionDecision(prevReps, currReps);
    int progressFull = progressionDecision(prevWeight, currWeight, prevReps, currReps);

    if (progressReps == 1 && progressFull == 1)
        advice += "You are progressing well in both strength and endurance. You can continue increasing intensity gradually.\n";
    else if (progressReps == -1 && progressFull == -1)
        advice += "Your workout progression has slowed across multiple areas. Consider reducing intensity slightly or focusing on recovery.\n";
    else
        advice += "Your workout progress is mixed. Some aspects are improving while others may need adjustment.\n";

    return advice;
}