#include "KitchenLib.h"
#include <iostream>

namespace KITCHENLIB {

    double outputAmount = 0.0;
    std::string outputUnit = "";
    std::string name = "";
    double amount = 0.0;
    std::string unit = "";

    double scaleAmount(double currentAmount, double factor) {
        return currentAmount * factor;
    }

    void convertToMetric(double amount, std::string unit){
        if (unit == "cups") {
            outputAmount = amount * 250;
            outputUnit = "ml";
        } else if (unit == "oz") {
            outputAmount = amount * 28.35;
            outputUnit = "grams";
        }
    }

    bool hasEnough(double pantryAmount, double recipeAmount) {
        return pantryAmount >= recipeAmount;
    }

    double buyIngredient(double amount, double bought) {
        return amount + bought;
    }

    bool isIngredientVegan(bool isAnimalProduct) {
        return !isAnimalProduct;
    }

    double calculateCalories(int grams, double caloriesPerGram) {
        return grams * caloriesPerGram;
    }

    double adjustServings(double totalVolume, int currentServings, int targetServings) {
        return totalVolume * ((double)targetServings / currentServings);
    }

    double calculateRemaining(double totalQuantity, double usedQuantity) {
        return totalQuantity - usedQuantity;
    }

    bool isLowFat(double totalWeight, double fatWeight) {
        return (fatWeight / totalWeight) <= 0.03;
    }

    bool canSubstitute(std::string original, std::string substitute, bool isCompatible) {
        return isCompatible;
    }

    bool isCookwareSafe(double currentTemp, double maxTemp) {
        return currentTemp < maxTemp;
    }

    void setOvenTemp(double temp) {
        std::cout << "Oven set to " << temp << "C\n";
    }

    void setOvenTemp(double temp, char unit) {
        if (unit == 'f' || unit == 'F') {
            temp = (temp - 32) * (5.0/9.0);
        }
        std::cout << "Oven set to " << temp << "C\n";
    }

    void printIngredientReport(std::string name, double amount, std::string unit) {
        std::cout << "Ingredient: " << name << " | Quantity: " << amount << " " << unit << "\n";
    }

    bool processIngredient(std::string name, double required, double pantry, std::string unit) {
        if (pantry >= required) {
            std::cout << name << " is ready for use!\n";
        } else {
            std::cout << "Missing " << (required - pantry) << unit << " of " << name << "\n";
        }
        return hasEnough(pantry, required);
    }

    void getIngredientInput() {
        std::cout << "Enter ingredient name: ";
        std::getline(std::cin, name);

        std::cout << "Enter amount and unit: ";
        std::cin >> amount >> unit;
    }

    void finalReport() {
        getIngredientInput();

        int factor;
        std::cout << "By how much do you need to increase the ingredient?\n";
        std::cin >> factor;
        double scaled = scaleAmount(amount, factor);

        double pantryAmount;
        std::cout << "How much of the product do you have in the pantry?\n";
        std::cin >> pantryAmount;
        bool enough = processIngredient(name, scaled, pantryAmount, unit);

        bool isAnimalProduct;
        std::cout << "Is the ingredient an animal product? (0 or 1)\n";
        std::cin >> isAnimalProduct;
        bool vegan = isIngredientVegan(isAnimalProduct);

        std::cout << "-----PRODUCT REPORT-----\n";
        printIngredientReport(name, amount, unit);
        std::cout << "Product scaled from " << amount << " to " << scaled << "\n";
        std::cout << "Has enough of the ingredient: " << enough << "\n";
        std::cout << "Is the ingredient vegan: " << vegan << "\n";
        std::cout << "------------------------\n";
    }
}