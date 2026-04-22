#include "SequenceOps.h"

long long fibonacciTerm(int n)
{
    if (n == 1 || n == 2)
        return 1;

    long long prev1 = 1;
    long long prev2 = 1;
    long long current;

    for (int i = 3; i <= n; i++)
    {
        current = prev1 + prev2;
        prev1 = prev2;
        prev2 = current;
    }

    return current;
}

long long fibonacciSum(int n) {
    long long sum = 0;
    for (int i = 0; i <= n; i++) {
        sum += fibonacciTerm(i);
    }
    return sum;
}

int collatzLength(int n) {
    int count = 0;
    while (n != 1) {
        if (n % 2 == 0) {
            n /= 2;
            count++;
        } else {
            n = 3 * n + 1;
            count++;
        }
    }
    return count;
}

long long sumOfRange(int start, int end) {
    long long sum = 0;
    for (int i = start; i <= end; i++) {
        sum += i;
    }
    return sum;
}

long long productOfRange(int start, int end) {
    long long product = start;
    for (int i = start + 1; i <=end; i++) {
        product *= i;
    }
    return product;
}