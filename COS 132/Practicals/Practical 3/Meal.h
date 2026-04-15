#ifndef MEAL_H
#define MEAL_H

#include <string>
#include <sstream>

using namespace std;

namespace Constants {
    const string KJ = "kJ";
    const string PROTEIN = "protein";
    const string CARBS = "carbs";
    const string FAT = "fat";
}

string makeMeal(const string name, double kilojoules, double proteinG, double carbsG, double fatG);
string getField(const string meal, const string field);
double getAmount(const string meal, const string field);
double kjPerGram(const string macro);

double macroRatio(const string meal, const string macro);    
bool isHighMacro(const string meal, const string macro);

void printMealInfo(const string meal);

#endif