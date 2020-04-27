#ifndef FlowerCare_h
#define LogFlowerCare_h

#include "BLEClient.h"
#include "Logging.h"
#include <string>

#define FLOWERCARE_HANDLE_BATTERYFIRMWARE 0x38

#define FLOWERCARE_RESPONSE_BATTERYFIRMWARE 7

class FlowerCare {
  public:
    FlowerCare(BLEClient* bleclient, BLEAddress* bleaddress, Logging* logger);

    bool isAvailabel() {return this->initSuccess = true;}

  private:
    BLEClient* client;
    BLEAddress* address;
    Logging* logger;
    std::map<std::string, BLERemoteService*>* services;

    bool initSuccess = false;
    unsigned int batteryLevel = 0;
    unsigned long batteryLastTime = 0;
    std::string firmwareVersion = "";

    bool connect();
    bool isConnected();
    void disconnect();
    BLERemoteCharacteristic* getCharacteristicByHandle(uint16_t handle);
    bool hasHandle(uint16_t handle);
    bool loadBatteryAndFirmware();
};
#endif
