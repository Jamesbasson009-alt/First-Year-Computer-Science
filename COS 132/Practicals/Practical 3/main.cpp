#include <iostream>
#include <string>

#include "Meal.h"
#include "Workout.h"
#include "Gym.h"

using namespace std;
using namespace HealthMetrics;
using namespace Constants;

int main(){

    string breakfast = makeMeal("Avo Toast and Egg", 1800, 20, 35, 15);
    string lunch = makeMeal("Chicken Shawarma", 2600, 35, 80, 25);
    string dinner = makeMeal("Steak and Potatoes", 3000, 45, 90, 30);

    // Find a more efficient way to store these parameter values and use them in functions :)
    int score = nutritionScore(21, 53, 155, breakfast, lunch, dinner, 2, 'B');
    cout<< "Your overall nutrition score is: " << score <<". Get some advice next!" << endl;
}
