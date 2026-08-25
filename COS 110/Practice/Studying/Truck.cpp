#include "Truck.h"

Truck::Truck() {
    numberOfDrivers = 0;
    numberOfParcels = 0;
    drivers = NULL;
    parcels = NULL;
}

Truck::~Truck() {
    delete[] drivers;
    delete[] parcels;
}

void Truck::addDriver(const Driver &driver) {
    Driver** newArr = new Driver*[numberOfDrivers + 1];
    for (int i = 0; i < numberOfDrivers; i++) {
        newArr[i] = drivers[i];
    }
    newArr[numberOfDrivers] = new Driver(driver);
    delete[] drivers;
    drivers = newArr;
    numberOfDrivers++;
}

void Truck::addParcel(const Parcel &parcel) {
    Parcel** newArr = new Parcel*[numberOfParcels + 1];
    for (int i = 0; i < numberOfParcels; i++) {
        newArr[i] = parcels[i];
    }
    newArr[numberOfParcels] = new Parcel(parcel);
    delete[] parcels;
    parcels = newArr;
    numberOfParcels++;
}
    
    