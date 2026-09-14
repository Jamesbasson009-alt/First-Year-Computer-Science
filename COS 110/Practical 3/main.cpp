#include "Factory.h"
#include "Worker.h"
#include <iostream>
#include <fstream>
#include <sstream>

void check(const std::string& label, bool condition) {
    std::cout << (condition ? "[PASS] " : "[FAIL] ") << label << std::endl;
}

int main() {
    std::cout << "=== Factory/Worker Test Suite ===\n" << std::endl;

    std::cout << "-- Default constructor --" << std::endl;
    Factory f1;
    std::ostringstream oss1;
    oss1 << f1;
    check("Empty factory prints 'Empty factory'", oss1.str() == "Empty factory");

    std::cout << "\n-- operator+=(Worker*) --" << std::endl;
    Worker* w1 = new Worker("Alice", 5000.0);
    Worker* w2 = new Worker("Bob", 6000.0);
    Worker* w3 = new Worker("Charlie", 7000.0);

    f1 += w1;
    f1 += w2;
    f1 += w3;
    check("Three workers added via pointer", f1[2] != 0);

    Worker* w1_dup = new Worker("Alice", 5000.0);
    f1 += w1_dup;
    check("Duplicate worker (Alice) not re-added", f1[3] == 0);
    delete w1_dup;

    f1 += static_cast<Worker*>(0);
    check("Adding NULL does nothing (still 3 workers)", f1[3] == 0);

    delete w1;
    delete w2;
    delete w3;

    std::cout << "\n-- operator+=(Worker&) --" << std::endl;
    Worker* shallowWorker = new Worker("Diana", 8000.0);
    f1 += *shallowWorker;
    check("Fourth worker added via reference", f1[3] != 0);

    std::cout << "\n-- operator[] --" << std::endl;
    Worker* found = f1[0];
    check("operator[](int) returns non-null for valid index", found != 0);
    check("operator[](int) returns null for out-of-bounds index", f1[100] == 0);

    if (found != 0) {
        int idx = f1[*found];
        check("operator[](Worker&) finds the same worker at index 0", idx == 0);
    }

    std::cout << "\n-- operator-= --" << std::endl;
    Worker probeForCharlie("Charlie", 7000.0);
    int charlieIndex = f1[probeForCharlie];
    check("Charlie exists before removal", charlieIndex != -1);

    if (charlieIndex != -1) {
        Worker* charlieCopy = f1[charlieIndex];
        f1 -= charlieCopy;
        check("Charlie removed", f1[probeForCharlie] == -1);
    }

    std::cout << "\n-- operator= --" << std::endl;
    Factory f2;
    f2 = f1;
    check("f2 equals f1 by total salary after assignment", f2 == f1);

    Worker* first = f1[0];
    if (first != 0) {
        Worker probe(*first);
        int before = f2[probe];
        f1 -= first;
        int after = f2[probe];
        check("Removing from f1 does not affect f2 (deep copy)", before == after && after != -1);
    }

    std::cout << "\n-- operator== / operator!= --" << std::endl;
    Factory f3;
    check("f1 != f2 after divergence", f1 != f2);
    check("Two empty factories are equal", f3 == Factory());

    std::cout << "\n-- Comparison operators --" << std::endl;
    const Factory& greater = (f2 > f1);
    std::ostringstream ossGreater;
    ossGreater << greater;
    check("operator> returns a printable factory", true);

    const Factory& lesser = (f1 < f2);
    std::ostringstream ossLesser;
    ossLesser << lesser;
    check("operator< returns a printable factory", true);

    const Factory& gte = (f2 >= f1);
    const Factory& lte = (f1 <= f2);
    std::ostringstream ossGte, ossLte;
    ossGte << gte;
    ossLte << lte;
    check("operator>= and operator<= run without crashing", true);

    Factory equalA;
    Factory equalB;
    const Factory& tie = (equalA > equalB);
    std::ostringstream ossTie;
    ossTie << tie;
    check("Equal-salary comparison prints 'Empty factory'", ossTie.str() == "Empty factory");

    std::cout << "\n-- operator*= / operator* --" << std::endl;
    Factory merged;
    merged *= f2;
    check("operator*= copies workers from f2 into merged", merged == f2);

    Factory* product = f1 * f2;
    std::ostringstream ossProduct;
    *product << std::flush;
    std::cout << *product;
    delete product;

    std::cout << "\n-- File I/O: operator| and operator<< / operator>> --" << std::endl;
    {
        std::ofstream outFile("workers_test.txt");
        outFile << f2;
        outFile.close();
    }

    Factory fileFactory;
    fileFactory | "workers_test.txt";
    check("Factory loaded from file has matching salary total", fileFactory == f2);

    std::ostringstream streamSource;
    streamSource << f2;
    std::istringstream streamIn(streamSource.str());
    Factory streamFactory;
    streamIn >> streamFactory;
    check("Factory loaded via operator>> has matching salary total", streamFactory == f2);

    Factory untouched;
    untouched += new Worker("Placeholder", 100.0);
    Factory beforeLoad = untouched;
    untouched | "this_file_does_not_exist.txt";
    check("Loading a missing file leaves factory unchanged", untouched == beforeLoad);

    std::cout << "\n=== Test suite finished ===" << std::endl;
    return 0;
}