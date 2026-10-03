#include "SmartLight.h"
#include <iostream>

SmartLight::SmartLight(std::string id, int initialBrightness)
    : SmartDevice(id), brightness(initialBrightness) {
}

SmartDevice* SmartLight::clone() const {
    return new SmartLight(*this);
}

SmartLight::~SmartLight() {
}

void SmartLight::executePrimaryFunction() {
    if (getIsOn()) {
        std::cout << "The light is ON at " << brightness << "%" << std::endl;
    }
}

std::string SmartLight::getDeviceType() const {
    return "Smart Light";
}

void SmartLight::dimLight(int percentage) {
    brightness -= percentage;
    if (brightness < 0) {
        brightness = 0;
    }
}

void SmartLight::printStatus() const {
    SmartDevice::printStatus();
    std::cout << "The brightness of the light: " << brightness << std::endl;
}
