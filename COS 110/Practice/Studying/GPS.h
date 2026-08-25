#include "Address.h"
#ifndef GPS_H
#define GPS_H

class GPS{

    private:
    static Address** addresses;
    static int numberOfAddresses;
    public:
    static const Address& obtainAddresses(std::string address);
    static void destroyAddresses();
};

#endif