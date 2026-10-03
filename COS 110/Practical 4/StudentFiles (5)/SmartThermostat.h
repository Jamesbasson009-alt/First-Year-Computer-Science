#ifndef SMARTTHERMOSTAT_H
#define SMARTTHERMOSTAT_H

#include "SmartDevice.h"

class SmartThermostat : public SmartDevice {
private:
    double currentTemp;
    double targetTemp;

public:
    SmartThermostat(std::string id, double targetT);
    virtual SmartDevice* clone() const;
    virtual ~SmartThermostat();

    virtual void executePrimaryFunction();
    virtual std::string getDeviceType() const;

    void setTemperature(double newTarget);
    void setTemperature(double newTarget, bool activateImmediately);

    virtual void printStatus() const;
};

#endif
