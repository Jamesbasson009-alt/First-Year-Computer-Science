#ifndef BASICMATH_H
#define BASICMATH_H

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

struct NegativeExponentError {
    const char* message;
};

struct ZeroDivisionError {
    const char* message;
};

struct InvalidRangeError {
    const char* message;
};

string calculate_summation(int lower, int upper);
string calculate_power(int base, int exponent);
string calculate_gcd(int a, int b);

#endif // BASICMATH_H