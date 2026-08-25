
#ifndef DRIVER_H
#define DRIVER_H
#include <string>
class Driver{

    private:

    std::string name;
    std::string lastName;
    char licenseCode;

    public:

    Driver(std::string fn, std::string ln, char lc);

};


#endif