#ifndef Sensor_FlowerCare_FlowerCare_h
#define Sensor_FlowerCare_FlowerCare_h

#include "esp32-hal.h"
#include <string>
#include "../../../SoilData.h"
#include "../../BLE/Interface.h"
#include "../../Logging/Interface.h"

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

namespace Sensor {
  namespace FlowerCare {
    class FlowerCare {
      public:
        FlowerCare(BLE::Interface* p_bleInterface, Logging::Interface* p_logger);
        ~FlowerCare();

        bool isAvailabel() {return m_initSuccess;}
        char* getFirmwareVersion() {return m_firmwareVersion;}
        unsigned int getBatteryLevel();
        char* getDeviceName() {return m_deviceName;}
        SoilData* getRealTimeData();
        bool blink();

      private:
        BLE::Interface* m_pBLEInterface;
        Logging::Interface* m_pLogger;
        
        char* m_identifier;
        bool m_initSuccess = false;
        unsigned int m_batteryLevel = 0;
        unsigned long m_batteryLastTime = 0;
        char* m_firmwareVersion;
        char* m_deviceName;
        SoilData* m_realTimeData;
        unsigned long m_realTimeDataLastTime = 0;
        static const char* p_stringInitializationSuccess;
        static const char* p_stringInitializationFail;
        static const char* p_stringBlinkSuccess;
        static const char* p_stringBlinkFail;
        static const char* p_stringDeviceUnavailable;
        static const char* p_stringDeviceResponseUnexpected;
        static const char* p_stringBatteryLevelFirmwareGet;
        static const char* p_stringBatteryLevelFirmwareInvalidBattery;
        static const char* p_stringBatteryLevel;
        static const char* p_stringFirmware;
        static const char* p_stringNameGet;
        static const char* p_stringNameInvalid;
        static const char* p_stringName;
        static const char* p_stringRealTimeDataSetMode;
        static const char* p_stringRealTimeDataGet;
        static const char* p_stringRealTimeData;

        bool loadBatteryAndFirmware();
        bool loadDeviceName();
        bool loadRealTimeData();
    };
  }
}
#endif
