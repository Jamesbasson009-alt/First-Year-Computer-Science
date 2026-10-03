#include <iostream>
#include "SmartDevice.h"
#include "SmartThermostat.h"
#include "SmartLight.h"
#include "SmartHomeManager.h"

using namespace std;

int main() {
    SmartHomeManager manager(3);
    manager.addDevice(new SmartThermostat("TH_01", 22.5));
    manager.addDevice(new SmartLight("LT_01", 75));
    manager.runSystemDiagnostic();
    SmartLight light();

    cout << endl;
    manager.activateAllDevices();
    cout << endl;
    cout << "Active devices: " << manager.getActiveDeviceCount() << endl;
    cout << endl;
    manager.runSystemDiagnostic();
    cout << endl;
    SmartHomeManager backupManager(manager);
    SmartDevice* removed = manager.removeDevice("LT_01");
    if (removed != NULL) {
        cout << "Removed device: " << removed->getDeviceID() << endl;
        delete removed;
    }
    cout << endl;
    manager.runSystemDiagnostic();
    cout << endl;
    backupManager.runSystemDiagnostic();
    cout << endl;
    manager.deactivateAllDevices();
    cout << "Active devices in main: " << manager.getActiveDeviceCount() << endl;
    cout << endl;
    cout << "Active devices in backup: " << backupManager.getActiveDeviceCount() << endl;
    return 0;
}