#include <string>
#include "Address.h"
#ifndef PARCEL_H
#define PARCEL_H

class Parcel{

    private:
    double weight;
    std::string description;
    bool fragile;
    const Address& address;
    bool delivered;

    public:
    Parcel(double weight, std::string description, bool fragile, const Address& address);
    bool markDelivered();
};

#endif