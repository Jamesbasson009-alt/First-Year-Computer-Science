#ifndef BUG_H
#define BUG_H
 
#include <string>

std::string getBugInfo(const std::string bugType);
int getBugDensity(const std::string bugType);
bool isSevereInfestation(const std::string bugType, int count);

#endif