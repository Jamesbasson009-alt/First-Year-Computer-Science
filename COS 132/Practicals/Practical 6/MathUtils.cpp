#include "MathUtils.h"

long long factorial(int n) {
    long long answer = 1;
    for (int i = 1; i <= n; i++) {
        answer *= i;
    }
    return answer;
}

double power(double base, int exp) {
    double answer = 1.0;
    if (exp == 0) {
        return 1.0;
    }
    for (int i = 0; i < exp; i++) {
        answer *= base;
    }
    return answer;
}

int sumDigits(int n) {
    int sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

int digitalRoot(int n) {
    while (n >= 10) {
        n = sumDigits(n);
    }
    return n;  
}

int countDivisors(int n) {
    int counter = 0;
    for (int i = 1; i <= n; i++) {
        if (n % i == 0) {
        counter++;
        }
    }
    return counter;
}


        