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

bool isGoalValid(char goal) {

    if (goal == 'B' || goal == 'C' || goal == 'T') {
        return true;
    }
    return false;
}

double kiloJouleTarget(double tdeeValue, char goal) {
    if (!isGoalValid(goal) || tdeeValue < 0) {
        return 0;
    }
    if (goal == 'B') {
        tdeeValue += 500;
    } else if (goal == 'C') {
        tdeeValue -= 500;
    }
    return tdeeValue;
}

double proteinTarget(double weightKg, char goal) {
    if (!isGoalValid(goal) || weightKg < 0) {
        return 0;
    }
    switch (goal)
    {
    case 'B':
        weightKg *= 2;
        break;

    case 'C':
        weightKg *= 1.2;
        break;

     case 'T':
        weightKg *= 0.8;
        break;
    
    default:
        break;
    }
    return weightKg;
}

int nutritionScore(int age, double weightKg, double heightM, const string m1, const string m2, const string m3, int activityLevel, char goal) {
    double totalKj = getAmount(m1, KJ) + getAmount(m2, KJ) + getAmount(m3, KJ);
    double totalProtein = getAmount(m1, PROTEIN) + getAmount(m2, PROTEIN) + getAmount(m3, PROTEIN);
    int score = 0;

    double tdeeValue = tdee(bmr(age, weightKg, heightM), activityLevel);
    double kjTarget = kiloJouleTarget(tdeeValue, goal);
    double pTarget = proteinTarget(weightKg, goal);

    if (totalKj >= kjTarget) {
        score += 2;
    } else if (totalKj >= kjTarget * 0.75) {
        score +=1;
    }

    if (totalProtein >= pTarget) {
        score += 2;
    } else if (totalProtein >= pTarget * 0.75) {
        score +=1;
    }

    if (goal == 'B') {
        if (isHighMacro(m1, PROTEIN)) score += 1;
        if (isHighMacro(m2, PROTEIN)) score += 1;
        if (isHighMacro(m3, PROTEIN)) score += 1;
    } else if (goal == 'C') {
        if (isHighMacro(m1, FAT)) score -= 1;
        if (isHighMacro(m2, FAT)) score -= 1;
        if (isHighMacro(m3, FAT)) score -= 1;
    }

    if (score < 0) {
        return 0;
    }
    return score;
}
