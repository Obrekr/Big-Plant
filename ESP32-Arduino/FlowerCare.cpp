#include "FlowerCare.h"

FlowerCare::FlowerCare(BLEInterface* p_bleInterface, Logging* p_logger) {
  m_pBLEInterface = p_bleInterface;
  m_pLogger = p_logger;

  // Try connecting to device
  if(!m_pBLEInterface->connect()) {
    m_pLogger->error(Logging::FLOWERCARE, "Initialization failed for " + m_pBLEInterface->getIdentifier());
    return;
  }

  // Check if all expected BLE handles are present
  if(!(m_pBLEInterface->hasHandle(FLOWERCARE_HANDLE_BATTERYFIRMWARE))) {
    m_pLogger->error(Logging::FLOWERCARE, "Initialization failed for " + m_pBLEInterface->getIdentifier());
    return;
  }
  
  // Get device firmware version
  if(!loadBatteryAndFirmware()) {
    m_pLogger->error(Logging::FLOWERCARE, "Initialization failed for " + m_pBLEInterface->getIdentifier());
    return;
  }

  m_pBLEInterface->disconnect();
  m_initSuccess = true;
  m_pLogger->info(Logging::FLOWERCARE, "Successfully initialized " + m_pBLEInterface->getIdentifier());
}

bool FlowerCare::loadBatteryAndFirmware() {
  char stringConvert[8];
  
  // Check if BLE client is connected
  if(!m_pBLEInterface->isConnected()) {
    m_pLogger->warning(Logging::FLOWERCARE, "Device " + m_pBLEInterface->getIdentifier() + "is currently unavailabel");
    return false;
  }

  // Read battery level and firmware version from BLE client
  m_pLogger->info(Logging::FLOWERCARE, "Getting battery level and firmware version from " + m_pBLEInterface->getIdentifier());
  std::string response = m_pBLEInterface->getValue(FLOWERCARE_HANDLE_BATTERYFIRMWARE);
  itoa(response.length(), stringConvert, 10);
  m_pLogger->debug(Logging::BLE, "Received " + std::string(stringConvert) + " bytes: " + response);

  // Check if response is plausible: fixed byte count received and magic byte 1 should be 0x13 or 0x28 or 0x2A
  if(!(response.length() == FLOWERCARE_RESPONSE_BATTERYFIRMWARE && (response.at(1) == 0x13 || response.at(1) == 0x28 || response.at(1) == 0x2A))) {
    m_pLogger->warning(Logging::FLOWERCARE, "Device " + m_pBLEInterface->getIdentifier() + " response is unexpected");
    return false;
  }

  // Extract battery level
  m_batteryLevel = response.at(0);
  if(m_batteryLevel > 100) {
    m_batteryLevel = 0;
    m_pLogger->warning(Logging::FLOWERCARE, "Device " + m_pBLEInterface->getIdentifier() + " reported invalid battery level");
    return false;
  }
  itoa(m_batteryLevel, stringConvert, 10);
  m_pLogger->info(Logging::FLOWERCARE, "Battery Level is " + std::string(stringConvert) + "%");

  // Extract firmware version
  m_firmwareVersion = response.substr(2, std::string::npos);
  m_pLogger->info(Logging::FLOWERCARE, "Firmware version is " + m_firmwareVersion);

  return true;
}
