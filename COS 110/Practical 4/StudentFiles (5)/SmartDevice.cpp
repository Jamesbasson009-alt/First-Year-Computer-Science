#include "SmartDevice.h"
#include <iostream>

SmartDevice::SmartDevice(std::string id) : deviceID(id), isOn(false) {
}

SmartDevice::~SmartDevice() {
}

bool SmartDevice::getIsOn() const {
    return isOn;
}

void SmartDevice::setIsOn(bool status) {
    isOn = status;
}

std::string SmartDevice::getDeviceID() const {
    return deviceID;
}

std::string SmartDevice::getDeviceType() const {
    return "Generic Smart Device";
}

void SmartDevice::printStatus() const {
    std::cout << "Device ID : " << deviceID << std::endl;
    std::cout << "The device is currently " << isOn << std::endl;
}
