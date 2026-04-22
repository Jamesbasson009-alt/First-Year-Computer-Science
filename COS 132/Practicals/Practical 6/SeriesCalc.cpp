#include "SeriesCalc.h"
#include "MathUtils.h"

double sinApprox(double x, int terms) {
    double sum = 0.0;
    for (int n = 0; n < terms; n++) {
        double sign = (n % 2 == 0) ? 1.0 : -1.0;
        double numerator = power(x, 2*n + 1);
        double denominator = factorial(2*n + 1);
        sum += sign * (numerator / denominator);
    }
    return sum;
}

double cosApprox(double x, int terms) {
    double sum = 0.0;
    for (int n = 0; n < terms; n++) {
        double sign = (n % 2 == 0) ? 1.0 : -1.0;
        double numerator = power(x, 2 * n);
        double denominator = factorial(2 * n);
        sum += sign * (numerator / denominator);
    }
    return sum;
}

double expApprox(double x, int terms) {
    double sum = 0.0;
    for (int n = 0; n < terms; n++) {
        sum += (power(x, n))/(factorial(n));
    }
    return sum;
}

double piApprox(int terms) {
    double sum = 0.0;
    for (int n = 0; n < terms; n++) {
        sum += (power(-1, n))/(2 * n +1);
    }
    return sum * 4;
}

