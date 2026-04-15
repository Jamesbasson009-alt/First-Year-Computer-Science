#include <cmath>

#include "House.h"

double roomObstacles(int id){

    switch (id)
    {
        case Kitchen::id:
            return Kitchen::obstacles;
            break;
        case LivingRoom::id:
            return LivingRoom::obstacles;
            break;
        case Bathroom::id:
            return Bathroom::obstacles;
            break;
        case BedroomOne::id:
            return BedroomOne::obstacles;
            break;
        case BedroomTwo::id:
            return BedroomTwo::obstacles;
            break;
        default:
            return 0.0;
    }
}

int roomSize(int id){

    switch (id)
    {
        case Kitchen::id:
            return Kitchen::size;
            break;
        case LivingRoom::id:
            return LivingRoom::size;
            break;
        case Bathroom::id:
            return Bathroom::size;
            break;
        case BedroomOne::id:
            return BedroomOne::size;
            break;
        case BedroomTwo::id:
            return BedroomTwo::size;
            break;
        default:
            return 0;
    }
}

int getEffectiveArea(double houseSize){

    int totalRoomSize = Kitchen::size + LivingRoom::size + Bathroom::size + BedroomOne::size + BedroomTwo::size;

    if (houseSize <= 0 || houseSize < totalRoomSize)
        return 0;

    double effective = houseSize;

    for (int i = 1; i <= 5; i++) {
        effective -= round(roomSize(i) * roomObstacles(i));
    }

    if (effective < 0)
        return 0;

    return (int)effective;
}
 
bool canFumigateRoom(int id){

    int size = roomSize(id);
    double obstacleD = roomObstacles(id);

    int clearSpace = size;

    int iterations = size / 10;

    for (int i = 0; i < iterations; i++) {

        if (obstacleD > 0.4) {
            clearSpace -= 10;
        }

        if (clearSpace < 0) {
            clearSpace = 0;
        }
    }

    return clearSpace > 20;
}
 
int countAccessibleRooms(){

    int accessible = 0;

    for (int i = 1; i < 6; i++) {
        if (canFumigateRoom(i)) accessible++;
    }
    
    return accessible;
}
 
int fumigate(double solution){

    int fumigated = 0;

    for (int id = 1; id <= 5; id++) {

        double cost = 0.4 * roomSize(id);

        if (solution < cost) {
            break;
        }

        solution -= cost;

        if (canFumigateRoom(id)) {
            fumigated++;
        }
    }

    return fumigated;
}