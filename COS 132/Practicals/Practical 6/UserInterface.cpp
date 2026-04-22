#include "UserInterface.h"
#include "MathUtils.h"
#include "SeriesCalc.h"
#include "SequenceOps.h"
#include <iostream>
#include <iomanip>

void displayFactorial() {
    int n;
    std::cout << "Enter n for factorial :" << std::endl;
    std::cin >> n;
    std::cout << n << "! = " << factorial(n) << std::endl;
}

void displaySinApprox() {
    double x;
    int terms;
    std::cout << "Enter x ( radians ) :" << std::endl;
    std::cin >> x;
    std::cout << "Enter number of terms :" << std::endl;
    std::cin >> terms;
    std::cout << "sin(" << std::fixed << std::setprecision(4) << x << ")~ "
              << std::setprecision(8) << sinApprox(x, terms) << std::endl;
}

void displayCosApprox() {
    double x;
    int terms;
    std::cout << "Enter x ( radians ) :" << std::endl;
    std::cin >> x;
    std::cout << "Enter number of terms :" << std::endl;
    std::cin >> terms;
    std::cout << "cos(" << std::fixed << std::setprecision(4) << x << ")~ "
              << std::setprecision(8) << cosApprox(x, terms) << std::endl;
}

void displayFibonacci() {
    int n;
    std::cout << "Enter n for Fibonacci :" << std::endl;
    std::cin >> n;
    std::cout << "F(" << n << ") = " << fibonacciTerm(n) << std::endl;
    std::cout << "Sum of first " << n << " Fibonacci numbers = " << fibonacciSum(n) << std::endl;
}

void displayCollatz() {
    int n;
    std::cout << "Enter starting number for Collatz :" << std::endl;
    std::cin >> n;
    std::cout << "Collatz sequence from " << n << " takes " 
              << collatzLength(n) << " steps to reach 1." << std::endl;
}