#ifndef TRUCK_H
#define TRUCK_H
#include "Driver.h"
#include "Parcel.h"
class Truck{
    private:
        Driver** drivers;
        int numberOfDrivers;
        Parcel** parcels;
        int numberOfParcels;

    public: 
        Truck();
        ~Truck();
        void addDriver(const Driver& driver);
        void addParcel(const Parcel& parcel);
        void deliverParcel(const Parcel& parcel);
};

#endif