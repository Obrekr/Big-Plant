#ifndef FlowerCare_h
#define LogFlowerCare_h

#include "BLEInterface.h"
#include "Logging.h"

#define FLOWERCARE_HANDLE_BATTERYFIRMWARE 0x38
#define FLOWERCARE_HANDLE_BLINK  0x33

#define FLOWERCARE_RESPONSE_BATTERYFIRMWARE 7

#define FLOWERCARE_REQUEST_BLINK 0xfdff

class FlowerCare {
  public:
    FlowerCare(BLEInterface* p_bleInterface, Logging* p_logger);

    bool isAvailabel() {return m_initSuccess;}
    bool blink();

  private:
    BLEInterface* m_pBLEInterface;
    Logging* m_pLogger;
    bool m_initSuccess = false;
    unsigned int m_batteryLevel = 0;
    unsigned long m_batteryLastTime = 0;
    std::string m_firmwareVersion = "";

    bool loadBatteryAndFirmware();
};
#endif
