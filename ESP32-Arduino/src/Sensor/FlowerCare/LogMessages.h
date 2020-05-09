#ifndef Sensor_FlowerCare_LogMessages_h
#define Sensor_FlowerCare_LogMessages_h

namespace Sensor {
  namespace FlowerCare {
    static const char* p_stringInitializationSuccess = "Successfully initialized %s";
    static const char* p_stringInitializationFail = "Initialization failed for %s";
    static const char* p_stringBlinkSuccess = "Successfully blinked %s";
    static const char* p_stringBlinkFail = "Failed to blink %s";
    static const char* p_stringDeviceUnavailable = "Device %s is currently unavailabel";
    static const char* p_stringDeviceResponseUnexpected = "Device %s response is unexpected";
    static const char* p_stringBatteryLevelFirmwareGet = "Getting battery level and firmware version from %s";
    static const char* p_stringBatteryLevelFirmwareInvalidBattery = "Device %s reported invalid battery level";
    static const char* p_stringBatteryLevel = "Battery Level is %u%%";
    static const char* p_stringFirmware = "Firmware version is %s";
    static const char* p_stringNameGet = "Getting device name from %s";
    static const char* p_stringNameInvalid = "Device %s reported invalid name";
    static const char* p_stringName = "Device Name is '%s'";
    static const char* p_stringRealTimeDataSetMode = "Failed to set %s to real time mode";
    static const char* p_stringRealTimeDataGet = "Getting real time data from %s";
    static const char* p_stringRealTimeData = "Real time data is %s";
  }
}
#endif