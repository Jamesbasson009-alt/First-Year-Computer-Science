#ifndef KITCHENLIB_H
#define KITCHENLIB_H

#include <string>

namespace KITCHENLIB {

    extern double outputAmount;
    extern std::string outputUnit;
    extern std::string name;
    extern double amount;
    extern std::string unit;


    double scaleAmount(double currentAmount, double factor);
    void convertToMetric(double amount, std::string unit);
    bool hasEnough(double pantryAmount, double recipeAmount);
    double buyIngredient(double amount, double bought);
    bool isIngredientVegan(bool isAnimalProduct);
    double calculateCalories(int grams, double caloriesPerGram);
    double adjustServings(double totalVolume, int currentServings, int targetServings);
    double calculateRemaining(double totalQuantity, double usedQuantity);
    bool isLowFat(double totalWeight, double fatWeight);
    bool canSubstitute(std::string orignal, std::string substitute, bool isCompatible);
    bool isCookwareSafe(double currentTemp, double maxTemp);
    void setOvenTemp(double temp);
    void setOvenTemp(double temp, char unit);
    void printIngredientReport(std::string name, double amount, std::string unit);
    bool processIngredient(std::string name, double required, double pantry, std::string unit);
    void getIngredientInput();
    void finalReport();
}



#endif