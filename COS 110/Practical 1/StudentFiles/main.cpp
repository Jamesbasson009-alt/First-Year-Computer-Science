#include "BasicMath.h"
#include "Permutations.h"

int main()
{
    char set1[] = {'a', 'b'};
    cout << printAllKLength(set1, 3, 2);
    cout << "==============================================" << endl;
    cout << calculate_summation(3, 5);
    cout << "==============================================" << endl;
    cout << calculate_power(3, 5);
    cout << "==============================================" << endl;
    cout << calculate_gcd(3, 5);
    cout << "==============================================" << endl;
    cout << printAllKLength(NULL, 2, 2);
    cout << "==============================================" << endl;
    char set2[] = {};
    cout << printAllKLength(set2, 2, 0);
    cout << "==============================================" << endl;
    char set3[] = {'a', 'b'};
    cout << printAllKLength(set3, -1, 2);
    cout << "==============================================" << endl;
    cout << calculate_summation(5, 4);
    cout << "==============================================" << endl;
    cout << calculate_summation(1, 1000000);
    cout << "==============================================" << endl;
    cout << calculate_power(5, -4);
    cout << "==============================================" << endl;
    cout << calculate_gcd(0, 0);
    cout << "==============================================" << endl;
    return 0;
}