#include "Meal.h"
#include <iostream>

using namespace Constants;

//DO NOT CHANGE
string makeMeal(const string name, double kilojoules, double protein, double carbs, double fat){
    ostringstream oss;
    oss << name << " | " << kilojoules << "kJ | " << protein << "g protein | " << carbs << "g carbs | " << fat << "g fat";
    return oss.str();
}

//DO NOT CHANGE
string getField(const string meal, const string field){
    if (field == "name")
    {
        size_t end = meal.find(" | ");
        if (end == string::npos) return "";
        return meal.substr(0, end);
    }

    string label;
    if(field == KJ)
        label = KJ;
    else if (field == PROTEIN)
        label = "g protein";
    else if (field == CARBS)
        label = "g carbs";
    else if (field == FAT)
        label = "g fat";
    else return "";

    size_t labelPos = meal.find(label);
    if (labelPos == string::npos) return "";

    size_t separatorPos = meal.rfind("| ", labelPos);
    if (separatorPos == string::npos) return "";

    size_t valueStart = separatorPos + 2;
    size_t valueEnd   = labelPos;

    return meal.substr(valueStart, valueEnd - valueStart);
}

//DO NOT CHANGE
double getAmount(const string meal, const string field){
    string raw = getField(meal, field);
    if (raw.empty()) 
        return 0.0;

    istringstream iss(raw);
    double value;
    if (iss >> value) 
        return value;

    return 0.0;
}

//DO NOT CHANGE
double kjPerGram(const string macro){
    double kj = 0.0;
 
    if (macro == PROTEIN)
        kj = 17.0;
    else if (macro == CARBS)
        kj = 17.0;
    else if (macro == FAT)
        kj = 37.0;
 
    return kj;
}

//DO NOT CHANGE
void printMealInfo(const string meal){
    cout << "Meal Name: " << getField(meal, "name") << endl;
    cout << "Energy (kJ): " << getAmount(meal, KJ) << endl;
    cout << "Protein (g): " << getAmount(meal, PROTEIN) << endl;
    cout << "Carbs (g): " << getAmount(meal, CARBS) << endl;
    cout << "Fat (g): " << getAmount(meal, FAT) << endl;
}

double macroRatio(const string meal, const string macro) {
    double kj = getAmount(meal, KJ);
    if (kj == 0) {
        return 0;
    }
    return (getAmount(meal, macro) * kjPerGram(macro)) / kj;
}

bool isHighMacro(const string meal, const string macro) {
    double ratio = macroRatio(meal, macro);
    
    if (macro == PROTEIN && ratio > 0.25) {
        return true;
    } else if (macro == CARBS && ratio > 0.5) {
        return true;
    } else if (macro == FAT && ratio > 0.3) {
        return true;
    }
    return false;
}
