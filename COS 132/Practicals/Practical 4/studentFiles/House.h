#ifndef HOUSE_H
#define HOUSE_H

#include <string>

double roomObstacles(int id);
int roomSize(int id);
int getEffectiveArea(double houseSize);
bool canFumigateRoom(int id);
int countAccessibleRooms();
int fumigate(double solution);


namespace Kitchen {
    const int id = 1;
    const int size = 70;
    const double obstacles = 0.8;
}

namespace LivingRoom {
    const int id = 2;
    const int size = 100;
    const double obstacles = 0.15;
}

namespace Bathroom {
    const int id = 3;
    const int size = 25;
    const double obstacles = 0.4;
}

namespace BedroomOne {
    const int id = 4;
    const int size = 60;
    const double obstacles = 0.6;
}

namespace BedroomTwo {
    const int id = 5;
    const int size = 40;
    const double obstacles = 0.2;
}
#endif