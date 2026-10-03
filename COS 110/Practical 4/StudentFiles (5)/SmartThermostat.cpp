#include "SmartThermostat.h"
#include <iostream>

namespace {
    
    double stepTowards(double value, double goal, double step) {
        if (value < goal) {
            value += step;
            if (value > goal) value = goal;
        } else if (value > goal) {
            value -= step;
            if (value < goal) value = goal;
        }
        return value;
    }
}

SmartThermostat::SmartThermostat(std::string id, double targetT)
    : SmartDevice(id), currentTemp(20), targetTemp(targetT) {
}

SmartDevice* SmartThermostat::clone() const {
    return new SmartThermostat(*this);
}

SmartThermostat::~SmartThermostat() {
}

void SmartThermostat::executePrimaryFunction() {
    if (getIsOn()) {
        currentTemp = stepTowards(currentTemp, targetTemp, 0.5);
    }
}

std::string SmartThermostat::getDeviceType() const {
    return "Smart Thermostat";
}

void SmartThermostat::setTemperature(double newTarget) {
    targetTemp = newTarget;
}

void SmartThermostat::setTemperature(double newTarget, bool activateImmediately) {
    if (activateImmediately) {
        targetTemp = newTarget;
    } else {
        targetTemp = stepTowards(targetTemp, newTarget, 0.5);
    }
}

void SmartThermostat::printStatus() const {
    SmartDevice::printStatus();
    std::cout << "The current temperature: " << currentTemp << std::endl;
    std::cout << "The target temparature: " << targetTemp << std::endl;
}
