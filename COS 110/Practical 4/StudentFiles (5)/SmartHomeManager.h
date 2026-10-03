#ifndef SMARTHOMEMANAGER_H
#define SMARTHOMEMANAGER_H

#include "SmartDevice.h"
#include <string>

class SmartHomeManager {
private:
    SmartDevice** devices;
    int deviceCount;
    int capacity;

public:
    SmartHomeManager(int maxCapacity);
    ~SmartHomeManager();
    SmartHomeManager(const SmartHomeManager& other);
    SmartHomeManager& operator=(const SmartHomeManager& other);

    void addDevice(SmartDevice* newDevice);
    void runSystemDiagnostic();
    void activateAllDevices();
    SmartDevice* removeDevice(std::string id);
    int getActiveDeviceCount() const;
    void deactivateAllDevices();
};

#endif
