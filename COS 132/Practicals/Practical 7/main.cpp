#include <iostream>
#include "StringLib.h"

int main() {
    char str1[] = "abc";
    char str2[] = "abba";
    char str3[] = "0523";
    char str4[] = "0523";

    char reversed[10];
    char copied[10];
    char combined[20];
    char sortedAsc[10];
    char sortedDesc[10];

    StringLib::print("abc");
    StringLib::print(NULL);

    std::cout << StringLib::length("abc") << std::endl;
    std::cout << StringLib::length(NULL) << std::endl;

    std::cout << StringLib::occurances("aab", 'a') << std::endl;
    std::cout << StringLib::occurances(NULL, 'a') << std::endl;

    const char* found = StringLib::find("abba", 'a', 2);

    if (found != NULL) {
        std::cout << *found << std::endl;
    }

    const char* notFound = StringLib::find("abba", 'z', 1);

    if (notFound == NULL) {
        std::cout << "NULL" << std::endl;
    }

    StringLib::reverse("abc", reversed);
    std::cout << reversed << std::endl;

    StringLib::copyOver("abc", copied);
    std::cout << copied << std::endl;

    StringLib::concatenate("abc", "efg", combined);
    std::cout << combined << std::endl;

    StringLib::replace(str2, 'a', 'e');
    std::cout << str2 << std::endl;

    StringLib::setTo(str1, '0');
    std::cout << str1 << std::endl;

    char smallestTest[] = "dbca";
    char largestTest[] = "dbca";

    std::cout << *StringLib::findSmallest(smallestTest) << std::endl;
    std::cout << *StringLib::findLargest(largestTest) << std::endl;

    StringLib::sortAscending(str3, sortedAsc);
    std::cout << sortedAsc << std::endl;

    StringLib::sortDescending(str4, sortedDesc);
    std::cout << sortedDesc << std::endl;

    return 0;
}