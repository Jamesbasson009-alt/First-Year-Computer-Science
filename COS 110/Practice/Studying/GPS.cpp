#include "GPS.h"


Address** GPS::addresses = NULL;
int GPS::numberOfAddresses = 0;

const Address& GPS::obtainAddresses(std::string address) {
    for (int i = 0; i < numberOfAddresses; i++) {
        if (address == addresses[i]->getAddress()) {
            return *addresses[i];
        }   
    }
    Address** newArr = new Address*[numberOfAddresses + 1];
    for (int i = 0; i < numberOfAddresses; i++) {
        newArr[i] = addresses[i];
    }
    newArr[numberOfAddresses] = new Address(address);
    numberOfAddresses++;
    delete[] addresses;
    addresses = newArr;
    return *addresses[numberOfAddresses - 1];
}

void GPS::destroyAddresses() {
    for (int i = 0; i < numberOfAddresses; i++) {
        
        delete addresses[i];
        addresses[i] = NULL;
    }
    
    delete[] addresses;
    addresses = NULL;
    numberOfAddresses = 0;
        
}
    