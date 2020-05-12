#include "FlowerCare.h"

namespace Sensor {
  namespace FlowerCare {
    const char* FlowerCare::p_stringInitializationSuccess = "Successfully initialized %s";
    const char* FlowerCare::p_stringInitializationFail = "Initialization failed for %s";
    const char* FlowerCare::p_stringBlinkSuccess = "Successfully blinked %s";
    const char* FlowerCare::p_stringBlinkFail = "Failed to blink %s";
    const char* FlowerCare::p_stringDeviceUnavailable = "Device %s is currently unavailabel";
    const char* FlowerCare::p_stringDeviceResponseUnexpected = "Device %s response is unexpected";
    const char* FlowerCare::p_stringBatteryLevelFirmwareGet = "Getting battery level and firmware version from %s";
    const char* FlowerCare::p_stringBatteryLevelFirmwareInvalidBattery = "Device %s reported invalid battery level";
    const char* FlowerCare::p_stringBatteryLevel = "Battery Level is %u%%";
    const char* FlowerCare::p_stringFirmware = "Firmware version is %s";
    const char* FlowerCare::p_stringNameGet = "Getting device name from %s";
    const char* FlowerCare::p_stringNameInvalid = "Device %s reported invalid name";
    const char* FlowerCare::p_stringName = "Device Name is '%s'";
    const char* FlowerCare::p_stringRealTimeDataSetMode = "Failed to set %s to real time mode";
    const char* FlowerCare::p_stringRealTimeDataGet = "Getting real time data from %s";
    const char* FlowerCare::p_stringRealTimeData = "Real time data is %s";
    
    FlowerCare::FlowerCare(BLE::Interface* p_bleInterface, Logging::Interface* p_logger)
                          : m_pBLEInterface(p_bleInterface), m_pLogger(p_logger) {
      m_identifier = m_pBLEInterface->getIdentifier();
      m_firmwareVersion = new char();
      m_deviceName = new char();
      
      // Try connecting to device
      if(!m_pBLEInterface->connect()) {
        goto fail;
      }

      // Check if all expected BLE handles are present
      if(!(m_pBLEInterface->hasHandle(FLOWERCARE_HANDLE_BATTERYFIRMWARE) && m_pBLEInterface->hasHandle(FLOWERCARE_HANDLE_DEVICENAME) 
           && m_pBLEInterface->hasHandle(FLOWERCARE_HANDLE_DEVICEMODE) && m_pBLEInterface->hasHandle(FLOWERCARE_HANDLE_REALTIMEDATA)))
      {
        goto fail;
      }
  
      // Get device firmware version and battery level
      if(!loadBatteryAndFirmware()) {
        goto fail;
      }

      // Get device name
      if(!loadDeviceName()) {
        goto fail;
      }

      // Init real time data and load from sensor
      m_realTimeData = new SoilData();
      if(m_realTimeData == 0 || !loadRealTimeData()) {
        goto fail;
      }

      m_pBLEInterface->disconnect();
      m_initSuccess = true;
      m_pLogger->info(p_stringInitializationSuccess, m_identifier);
      return;

    fail:
      m_pBLEInterface->disconnect();
      m_pLogger->error(p_stringInitializationFail, m_identifier);
    }

    FlowerCare::~FlowerCare() {
      delete[] m_firmwareVersion;
      delete[] m_deviceName;
      delete m_realTimeData;
    }

    unsigned int FlowerCare::getBatteryLevel() {
      if((millis() - m_batteryLastTime > FLOWERCARE_BATTERY_VALIDTIME * 1000L) && m_pBLEInterface->connect()) {
        loadBatteryAndFirmware();
        m_pBLEInterface->disconnect();
      }
      return m_batteryLevel;
    }

    SoilData* FlowerCare::getRealTimeData() {
      if((millis() - m_realTimeDataLastTime > FLOWERCARE_REALTIMEDATA_VALIDTIME * 1000L) && m_pBLEInterface->connect()) {
        loadRealTimeData();
        m_pBLEInterface->disconnect();
      }
      return m_realTimeData;
    }

    bool FlowerCare::blink() {
      // Connect to device
      if(!m_pBLEInterface->connect()) {
        m_pLogger->warning(p_stringBlinkFail, m_identifier);
        return false;
      }

      // Transmit 2 magic bytes to FLOWERCARE_HANDLE_DEVICEMODE to blink the LED
      char transmitData[2] = {FLOWERCARE_REQUEST_BLINK && 0xFF, (FLOWERCARE_REQUEST_BLINK >> 8) && 0xFF};
      if(!m_pBLEInterface->setValue(FLOWERCARE_HANDLE_DEVICEMODE, std::string(transmitData))) {
        m_pLogger->warning(p_stringBlinkFail, m_identifier);
        return false;
      }
  
      m_pLogger->info(p_stringBlinkSuccess, m_identifier);
      return true;
    }

    bool FlowerCare::loadBatteryAndFirmware() {
      // Check if BLE client is connected
      if(!m_pBLEInterface->isConnected()) {
        m_pLogger->warning(p_stringDeviceUnavailable, m_identifier);
        return false;
      }

      // Read battery level and firmware version from BLE client
      m_pLogger->info(p_stringBatteryLevelFirmwareGet, m_identifier);
      std::string response = m_pBLEInterface->getValue(FLOWERCARE_HANDLE_BATTERYFIRMWARE);

      // Check if response is plausible: fixed byte count received and magic byte 1 should be 0x13 or 0x28 or 0x2A
      if(!(response.length() == FLOWERCARE_RESPONSE_BATTERYFIRMWARE && (response.at(1) == 0x13 || response.at(1) == 0x28 || response.at(1) == 0x2A))) {
        m_pLogger->warning(p_stringDeviceResponseUnexpected, m_identifier);
        return false;
      }

      // Extract battery level
      m_batteryLevel = response.at(0);
      if(m_batteryLevel > 100) {
        m_batteryLevel = 0;
        m_pLogger->warning(p_stringBatteryLevelFirmwareInvalidBattery, m_identifier);
        return false;
      }
      m_pLogger->info(p_stringBatteryLevel, m_batteryLevel);

      // Extract firmware version
      delete[] m_firmwareVersion;
      m_firmwareVersion = new char[response.substr(2, std::string::npos).length() + 1];
      std::strcpy(m_firmwareVersion, response.substr(2, std::string::npos).c_str());
      m_pLogger->info(p_stringFirmware, m_firmwareVersion);

      m_batteryLastTime = millis();
      return true;
    }

    bool FlowerCare::loadDeviceName() {
      // Check if BLE client is connected
      if(!m_pBLEInterface->isConnected()) {
        m_pLogger->warning(p_stringDeviceUnavailable, m_identifier);
        return false;
      }

      // Read device name from BLE client
      m_pLogger->info(p_stringNameGet, m_identifier);
      std::string response = m_pBLEInterface->getValue(FLOWERCARE_HANDLE_DEVICENAME);

      if(response.length() == 0) {
        m_pLogger->warning(p_stringNameInvalid, m_identifier);
        return false;
      }
      
      // Save device name
      delete[] m_deviceName;
      m_deviceName = new char[response.length() + 1];
      std::strcpy(m_deviceName, response.c_str());
      
      m_pLogger->info(p_stringName, m_deviceName);
      return true;
    }

    bool FlowerCare::loadRealTimeData() {
      // Check if BLE client is connected
      if(!m_pBLEInterface->isConnected()) {
        m_pLogger->warning(p_stringDeviceUnavailable,m_identifier);
        return false;
      }

      // Transmit 2 magic bytes to FLOWERCARE_HANDLE_DEVICEMODE to be able to read real time data
      char transmitData[2] = {FLOWERCARE_REQUEST_REALTIMEDATA && 0xFF, (FLOWERCARE_REQUEST_REALTIMEDATA >> 8) && 0xFF};
      if(!m_pBLEInterface->setValue(FLOWERCARE_HANDLE_DEVICEMODE, std::string(transmitData))) {
        m_pLogger->warning(p_stringRealTimeDataSetMode, m_identifier);
        return false;
      }

      // Read real time data
      m_pLogger->info(p_stringRealTimeDataGet, m_identifier);
      std::string response = m_pBLEInterface->getValue(FLOWERCARE_HANDLE_REALTIMEDATA);

      // Check if response has right length
      if(response.length() != FLOWERCARE_RESPONSE_REALTIMEDATA) {
        m_pLogger->warning(p_stringDeviceResponseUnexpected, m_identifier);
        return false;
      }

      // Extract soil data
      m_realTimeData->setTemperature((response.at(0) | response.at(1) >> 8));
      m_realTimeData->setBrightness((response.at(3) | response.at(4) >> 8) | (response.at(5) >> 16 | response.at(6) >> 24));
      m_realTimeData->setMoisture(response.at(7));
      m_realTimeData->setConductivity((response.at(8) | response.at(9) >> 8));
      m_pLogger->info(p_stringRealTimeData, m_realTimeData->toString());

      m_realTimeDataLastTime = millis();
      return true;
    }
  }
}
