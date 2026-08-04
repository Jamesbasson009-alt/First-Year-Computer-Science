#include "BasicMath.h"

static string getIndent(int level)
{
    if (level <= 0)
    {
        return "";
    }

    return "  " + getIndent(level - 1);
}

static string intToString(int num)
{
    stringstream ss;
    ss << num;
    return ss.str();
}

static int calculate_summation(int lower, int upper, int level, string &trace)
{

    trace += getIndent(level) + "Calling Summation(" + intToString(lower) + ", " + intToString(upper) + ")\n";

    if (lower > upper)
    {
        trace += getIndent(level) + "Base case: return 0\n";
        return 0;
    }

    int subResult = calculate_summation(lower + 1, upper, level + 1, trace);
    int result = lower + subResult;

    trace += getIndent(level) + "Returning " + intToString(lower) + " + " + intToString(subResult) + " = " + intToString(result) + "\n";

    return result;
}

string calculate_summation(int lower, int upper)
{

    try
    {
        if (lower > upper)
        {
            InvalidRangeError e;
            e.message = "Caught exception: Lower bound cannot be greater than upper bound\n";
            throw e;
        }
        if (upper - lower + 1 > 10000)
        {
            InvalidRangeError e;
            e.message = "Caught exception: Range too large (max 10000)\n";
            throw e;
        }

        std::string trace;
        int result = calculate_summation(lower, upper, 0, trace);
        trace += "\nResult: " + intToString(result) + "\n";
        return trace;
    }
    catch (InvalidRangeError &e)
    {
        return e.message;
    }
}

static int calculate_power(int base, int exponent, int level, string &trace)
{
    trace += getIndent(level) + "Calling power(" + intToString(base) + ", " + intToString(exponent) + ")\n";

    if (exponent == 0)
    {
        trace += getIndent(level) + "Base case: power(" + intToString(base) + ", 0) = 1\n";
        return 1;
    }

    int subResult = calculate_power(base, exponent - 1, level + 1, trace);
    int result = base * subResult;

    trace += getIndent(level) + "Returning " + intToString(base) + " * " + intToString(subResult) + " = " + intToString(result) + " for power(" + intToString(base) + ", " + intToString(exponent) + ")\n";

    return result;
}

string calculate_power(int base, int exponent)
{
    try
    {
        if (exponent < 0)
        {
            NegativeExponentError e;
            e.message = "Caught exception: Negative exponent not supported\n";
            throw e;
        }

        std::string trace;
        int result = calculate_power(base, exponent, 0, trace);
        trace += "\nResult: " + intToString(result) + "\n";
        return trace;
    }
    catch (NegativeExponentError &e)
    {
        return e.message;
    }
}

static int calculate_gcd(int a, int b, int level, string &trace)
{

    trace += getIndent(level) + "Calling gcd(" + intToString(a) + ", " + intToString(b) + ")\n";

    if (b == 0)
    {

        trace += getIndent(level) + "Base case: gcd(" + intToString(a) + ", 0) = " + intToString(a) + "\n";
        return a;
    }

    int subA = b;
    int subB = a % b;
    int result = calculate_gcd(subA, subB, level + 1, trace);

    trace += getIndent(level) + "Returning gcd(" + intToString(subA) + ", " + intToString(subB) + ") = " + intToString(result) + " for gcd(" + intToString(a) + ", " + intToString(b) + ")\n";

    return result;
}

string calculate_gcd(int a, int b)
{
    try
    {
        if (a == 0 && b == 0)
        {
            ZeroDivisionError e;
            e.message = "Caught exception: Both numbers cannot be zero\n";
            throw e;
        }

        if (a < 0)
        {
            a = -a;
        }
        if (b < 0)
        {
            b = -b;
        }

        std::string trace;
        int result = calculate_gcd(a, b, 0, trace);
        trace += "\nResult: " + intToString(result) + "\n";
        return trace;
    }
    catch (ZeroDivisionError &e)
    {
        return e.message;
    }
}



