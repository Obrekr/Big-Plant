#ifndef FlowerCare_h
#define LogFlowerCare_h

#include "BLEClient.h"
#include "Logging.h"
#include <string>

#define FLOWERCARE_CHARACTERISTICSCOUNT 25
#define FLOWERCARE_HANDLE_BATTERYFIRMWARE 0x38

#define FLOWERCARE_RESPONSE_BATTERYFIRMWARE 7

class FlowerCare {
  public:
    FlowerCare(BLEClient* bleclient, BLEAddress* bleaddress, Logging* logger);

    bool isAvailabel() {return this->deviceAvailable;}

  private:
    BLEClient* client;
    BLEAddress* address;
    Logging* logger;
    std::map<uint16_t, BLERemoteCharacteristic*> characteristics;

    bool deviceAvailable = false;
    unsigned int batteryLevel = 0;
    unsigned long batteryLastTime = 0;
    std::string firmwareVersion = "";

    bool connect();
    void disconnect();
    void loadBatteryAndFirmware();
};
#endif
