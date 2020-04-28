#ifndef FlowerCare_h
#define LogFlowerCare_h

#include "BLEInterface.h"
#include "SoilData.h"
#include "Logging.h"

#define FLOWERCARE_HANDLE_BATTERYFIRMWARE 0x38
#define FLOWERCARE_HANDLE_DEVICENAME 0x03
#define FLOWERCARE_HANDLE_DEVICEMODE  0x33
#define FLOWERCARE_HANDLE_REALTIMEDATA 0x35

#define FLOWERCARE_RESPONSE_BATTERYFIRMWARE 7
#define FLOWERCARE_RESPONSE_REALTIMEDATA 16

#define FLOWERCARE_REQUEST_BLINK 0xfdff
#define FLOWERCARE_REQUEST_REALTIMEDATA 0xA01F

#define FLOWERCARE_BATTERY_VALIDTIME 3600
#define FLOWERCARE_REALTIMEDATA_VALIDTIME 900

class FlowerCare {
  public:
    FlowerCare(BLEInterface* p_bleInterface, Logging* p_logger);
    ~FlowerCare();

    bool isAvailabel() {return m_initSuccess;}
    std::string getFirmwareVersion() {return m_firmwareVersion;}
    unsigned int getBatteryLevel();
    std::string getDeviceName() {return m_deviceName;}
    SoilData* getRealTimeData();
    bool blink();

  private:
    BLEInterface* m_pBLEInterface;
    Logging* m_pLogger;
    bool m_initSuccess = false;
    unsigned int m_batteryLevel = 0;
    unsigned long m_batteryLastTime = 0;
    std::string m_firmwareVersion = "";
    std::string m_deviceName = "";
    SoilData* m_realTimeData;
    unsigned long m_realTimeDataLastTime = 0;

    bool loadBatteryAndFirmware();
    bool loadDeviceName();
    bool loadRealTimeData();
};
#endif
