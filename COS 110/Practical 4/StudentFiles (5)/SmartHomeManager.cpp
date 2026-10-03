#include "SmartHomeManager.h"


SmartHomeManager::SmartHomeManager(int maxCapacity)
    : devices(NULL), deviceCount(0), capacity(maxCapacity < 1 ? 1 : maxCapacity) {
    devices = new SmartDevice*[capacity];
    for (int i = 0; i < capacity; i++) {
        devices[i] = NULL;
    }
}

SmartHomeManager::~SmartHomeManager() {
    for (int i = 0; i < deviceCount; i++) {
        delete devices[i];
    }
    delete[] devices;
}

SmartHomeManager::SmartHomeManager(const SmartHomeManager& other)
    : devices(NULL), deviceCount(other.deviceCount), capacity(other.capacity) {
    devices = new SmartDevice*[capacity];
    for (int i = 0; i < capacity; i++) {
        if (i < deviceCount && other.devices[i] != NULL) {
            devices[i] = other.devices[i]->clone();
        } else {
            devices[i] = NULL;
        }
    }
}

SmartHomeManager& SmartHomeManager::operator=(const SmartHomeManager& other) {
    if (this != &other) {
        for (int i = 0; i < deviceCount; i++) {
            delete devices[i];
        }
        delete[] devices;

        capacity = other.capacity;
        deviceCount = other.deviceCount;
        devices = new SmartDevice*[capacity];
        for (int i = 0; i < capacity; i++) {
            if (i < deviceCount && other.devices[i] != NULL) {
                devices[i] = other.devices[i]->clone();
            } else {
                devices[i] = NULL;
            }
        }
    }
    return *this;
}

void SmartHomeManager::addDevice(SmartDevice* newDevice) {
    if (newDevice == NULL || deviceCount >= capacity) {
        return;
    }
    devices[deviceCount] = newDevice;   // shallow copy
    deviceCount++;
}

void SmartHomeManager::runSystemDiagnostic() {
    for (int i = 0; i < deviceCount; i++) {
        if (devices[i] != NULL) {
            devices[i]->printStatus();
        }
    }
}

void SmartHomeManager::activateAllDevices() {
    for (int i = 0; i < deviceCount; i++) {
        if (devices[i] != NULL) {
            devices[i]->setIsOn(true);
            devices[i]->executePrimaryFunction();
        }
    }
}

SmartDevice* SmartHomeManager::removeDevice(std::string id) {
    for (int i = 0; i < deviceCount; i++) {
        if (devices[i] != NULL && devices[i]->getDeviceID() == id) {
            SmartDevice* removed = devices[i];
            // shift left so gaps (NULLs) stay at the end
            for (int j = i; j < deviceCount - 1; j++) {
                devices[j] = devices[j + 1];
            }
            devices[deviceCount - 1] = NULL;
            deviceCount--;
            return removed;
        }
    }
    return NULL;
}

int SmartHomeManager::getActiveDeviceCount() const {
    int count = 0;
    for (int i = 0; i < deviceCount; i++) {
        if (devices[i] != NULL && devices[i]->getIsOn()) {
            count++;
        }
    }
    return count;
}

void SmartHomeManager::deactivateAllDevices() {
    for (int i = 0; i < deviceCount; i++) {
        if (devices[i] != NULL) {
            devices[i]->setIsOn(false);
        }
    }
}
