#include "Parcel.h"
#include "Address.h"
Parcel::Parcel(double weight, std::string description, bool fragile, const Address& address) : address(address) {
    this->weight = weight;
    this->description = description;
    this->fragile = fragile;
    delivered = false;
}

bool Parcel::markDelivered() {
    delivered = true;
    return delivered;
}
    