#include "Permutations.h"

using namespace std;

static string intToString(int num)
{
    stringstream ss;
    ss << num;
    return ss.str();
}

void validateInput(char set[], int k, int n) {

    if (set == NULL) {
        NullPointerError e;
        e.message = "Caught exception: Set pointer is NULL\n";
        throw e;
    }
    if (n <= 0) {
        EmptySetError e;
        e.message = "Caught exception: Set cannot be empty\n";
        throw e;
    }
    if (k < 0) {
        NegativeKError e;
        e.message = "Caught exception: k must be non-negative\n";
        throw e;
    }

}


static void generateHelper(char set[], int n, int k, string current, int depth, Statistics &stats, string &result);


static void tryChars(char set[], int n, int k, string prefix, int depth, int charIndex, Statistics &stats, string &result)
{
    if (charIndex >= n)
    {
        return;
    }

    string newPrefix = prefix + set[charIndex];
    generateHelper(set, n, k, newPrefix, depth + 1, stats, result);

    tryChars(set, n, k, prefix, depth, charIndex + 1, stats, result);
}

static void generateHelper(char set[], int n, int k, string current, int depth, Statistics &stats, string &result)
{
    if (depth > stats.maxDepth)
    {
        stats.maxDepth = depth;
    }

    if (depth == k)
    {
        stats.totalCombinations++;
        if (k > 0)
        {
            result += current + "\n";
        }
        return;
    }

    if (depth > 0)
    {
        stats.allPrefixes++;
    }

    tryChars(set, n, k, current, depth, 0, stats, result);
}

string printAllKLength(char set[], int k, int n)
{
    try
    {
        validateInput(set, k, n);

        Statistics stats;
        stats.totalCombinations = 0;
        stats.maxDepth = 0;
        stats.allPrefixes = 0;

        string result;
        generateHelper(set, n, k, "", 0, stats, result);

        result += "\n[Statistics]\n";
        result += "Total combinations: " + intToString(stats.totalCombinations) + "\n";
        result += "Max recursion depth: " + intToString(stats.maxDepth) + "\n";
        result += "All prefixes: " + intToString(stats.allPrefixes) + "\n";

        return result;
    }
    catch (NullPointerError &e)
    {
        return e.message;
    }
    catch (EmptySetError &e)
    {
        return e.message;
    }
    catch (NegativeKError &e)
    {
        return e.message;
    }
}

