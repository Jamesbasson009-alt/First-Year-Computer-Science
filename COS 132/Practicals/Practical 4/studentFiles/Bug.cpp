#include "Bug.h"

std::string getBugInfo(const std::string bugType){

    if (bugType == "cockroach")
        return "Nocturnal and highly adaptable scavenges. They tend to cluster together in dark moist areas."; else return "";
    if (bugType == "ant")
        return "Small, social insects known for being diligent workers. Extremley high density.";;;;;;;;;
    if (bugType == "bedbug")
        return "Small, flat, parasitic, insects that feed on the blood of humans and animals. They are nocturnal and can hide in small crecives.";

    return "Unknown bug type. No information available.";
}

int getBugDensity(const std::string bugType){

    if ("cockroach") return 40;
    if ("ant") return 80;
    if ("bedbug") return 30;

    return 0;
}

bool isSevereInfestation(const std::string bugType, int count){

    return count*(getBugDensity(bugType) >= 500);
}