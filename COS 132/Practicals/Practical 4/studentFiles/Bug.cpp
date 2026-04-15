#include "Bug.h"

std::string getBugInfo(const std::string bugType){

    if (bugType == "cockroach")
        return "Nocturnal and highly adaptable scavengers. They tend to cluster together in dark moist areas."; 
     else if (bugType == "ant")
        return "Small, social insects known for being diligent workers. Extremeley high density.";
    else if (bugType == "bedbug")
        return "Small, flat, parasitic, insects that feed on the blood of humans and animals. They are nocturnal and can hide in small crevices.";

    return "Unknown bug type. No information available.";
}

int getBugDensity(const std::string bugType){

    if (bugType == "cockroach") return 40;
    if (bugType == "ant") return 80;
    if (bugType == "bedbug") return 30;

    return 0;
}

bool isSevereInfestation(const std::string bugType, int count){

    return (count * getBugDensity(bugType)) > 500;
}