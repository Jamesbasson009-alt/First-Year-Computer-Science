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
        trace += "Result: " + intToString(result) + "\n";
        return trace;
    }
    catch (InvalidRangeError &e)
    {
        return e.message;
    }
}

static int calculate_power(int base, int exponent, int level, string &trace)
{

    trace += getIndent(level) + "Calling Power(" + intToString(base) + ", " + intToString(exponent) + ")\n";

    if (exponent == 0)
    {
        trace += getIndent(level) + "Base case: return 1\n";
        return 1;
    }

    int subResult = calculate_power(base, exponent - 1, level + 1, trace);
    int result = base * subResult;

    trace += getIndent(level) + "Returning " + intToString(base) + " * " + intToString(subResult) + " = " + intToString(result) + "\n";

    return result;
}

string calculate_power(int base, int exponent) {
    
}
