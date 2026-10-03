#ifndef SMARTLIGHT_H
#define SMARTLIGHT_H

#include "SmartDevice.h"

class SmartLight : public SmartDevice {
private:
    int brightness;

public:
    SmartLight(std::string id, int initialBrightness);
    virtual SmartDevice* clone() const;
    virtual ~SmartLight();

    virtual void executePrimaryFunction();
    virtual std::string getDeviceType() const;

    void dimLight(int percentage);

    virtual void printStatus() const;
};

#endif
