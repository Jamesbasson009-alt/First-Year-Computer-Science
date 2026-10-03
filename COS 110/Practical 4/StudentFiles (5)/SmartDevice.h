#ifndef SMARTDEVICE_H
#define SMARTDEVICE_H

#include <string>

class SmartDevice {
private:
    std::string deviceID;
    bool isOn;

public:
    SmartDevice(std::string id);
    virtual SmartDevice* clone() const = 0;
    virtual ~SmartDevice();

    virtual void executePrimaryFunction() = 0;

    bool getIsOn() const;
    void setIsOn(bool status);
    std::string getDeviceID() const;

    virtual std::string getDeviceType() const;
    virtual void printStatus() const;
};

#endif
