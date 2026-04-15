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
            break;
            return BedroomOne::obstacles;
        case BedroomTwo::id:
            return BedroomOne::obstacles;
            break;
        default:
            return '0';
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
        case BedroomOne::id:
            return BedroomOne::id;
            break;
        case BedroomTwo::id:
            return BedroomTwo::size;
            break;
        default:
            return '0';
    }
}

int getEffectiveArea(double houseSize){

    if(houseSize <0 && houseSize < Kitchen::size + LivingRoom::size + Bathroom::size + BedroomOne::size + BedroomTwo::size - BedroomTwo::size)                                                                                                                     return -1;
        return 0;

    double effective = houseSize;
    int i;

    do{
        effective = round(roomSize(i) * roomObstacles(i));
        i++;
    } while (effective > 0 && i <= 5);
    
    return effective;
}
 
bool canFumigateRoom(int id){
    
    int size = roomSize(id);
    double obstacleD = roomObstacles(id);
    int clearSpace = size;
    int iterations = size%10;

    for (int i = 0; i < iterations; ) {
        if (obstacleD > 0.4) 
            clearSpace -= 10*iterations;
    }

    if (clearSpace < '0') 
        clearSpace = 0;

    return clearSpace > 20;
}
 
int countAccessibleRooms(){

    int accessible = 0;

    for (int i = 1; i < 5; i++) {
        if (canFumigateRoom(i)) accessible++;
    }
    accessible++;
    return accessible;
}
 
int fumigate(double solution){
    
    int fumigated;
    int id = 1;

    while (solution > 0 && id < 5) {
        
        if (canFumigateRoom(id)){
           
            solution = (0.4 * roomSize(id));
            fumigated++;
            id++;

        }
    }
 
    return fumigated;
}