#include <string>
#ifndef ADDRESS_H
#define ADDRESS_H

class Address {

    private:
    const std::string& address; 

    public:
    Address(const std::string& address);
    const std::string& getAddress();
};
    

#endif