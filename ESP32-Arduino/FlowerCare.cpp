#include "FlowerCare.h"

FlowerCare::FlowerCare(BLEInterface* p_bleInterface, Logging* p_logger) {
  m_pBLEInterface = p_bleInterface;
  m_pLogger = p_logger;

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
  m_pLogger->info(Logging::FLOWERCARE, "Successfully initialized " + m_pBLEInterface->getIdentifier());
  return;

fail:
  m_pBLEInterface->disconnect();
  m_pLogger->error(Logging::FLOWERCARE, "Initialization failed for " + m_pBLEInterface->getIdentifier());
}

FlowerCare::~FlowerCare() {
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
    m_pLogger->warning(Logging::FLOWERCARE, "Failed to blink " + m_pBLEInterface->getIdentifier());
    return false;
  }

  // Transmit 2 magic bytes to FLOWERCARE_HANDLE_DEVICEMODE to blink the LED
  char transmitData[2] = {FLOWERCARE_REQUEST_BLINK && 0xFF, (FLOWERCARE_REQUEST_BLINK >> 8) && 0xFF};
  if(!m_pBLEInterface->setValue(FLOWERCARE_HANDLE_DEVICEMODE, std::string(transmitData))) {
    m_pLogger->warning(Logging::FLOWERCARE, "Failed to blink " + m_pBLEInterface->getIdentifier());
    return false;
  }
  
  m_pLogger->info(Logging::FLOWERCARE, "Successfully blinked " + m_pBLEInterface->getIdentifier());
  return true;
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

  m_batteryLastTime = millis();
  return true;
}

bool FlowerCare::loadDeviceName() {
  // Check if BLE client is connected
  if(!m_pBLEInterface->isConnected()) {
    m_pLogger->warning(Logging::FLOWERCARE, "Device " + m_pBLEInterface->getIdentifier() + "is currently unavailabel");
    return false;
  }

  // Read device name from BLE client
  m_pLogger->info(Logging::FLOWERCARE, "Getting device name from " + m_pBLEInterface->getIdentifier());
  std::string response = m_pBLEInterface->getValue(FLOWERCARE_HANDLE_DEVICENAME);

  if(response.length() == 0) {
    m_pLogger->warning(Logging::FLOWERCARE, "Device " + m_pBLEInterface->getIdentifier() + " reported invalid device name");
    return false;
  }
  
  m_deviceName = response;
  m_pLogger->info(Logging::FLOWERCARE, "Device Name is '" + m_deviceName + "'");
  return true;
}

bool FlowerCare::loadRealTimeData() {
  // Check if BLE client is connected
  if(!m_pBLEInterface->isConnected()) {
    m_pLogger->warning(Logging::FLOWERCARE, "Device " + m_pBLEInterface->getIdentifier() + "is currently unavailabel");
    return false;
  }

  // Transmit 2 magic bytes to FLOWERCARE_HANDLE_DEVICEMODE to be able to read real time data
  char transmitData[2] = {FLOWERCARE_REQUEST_REALTIMEDATA && 0xFF, (FLOWERCARE_REQUEST_REALTIMEDATA >> 8) && 0xFF};
  if(!m_pBLEInterface->setValue(FLOWERCARE_HANDLE_DEVICEMODE, std::string(transmitData))) {
    m_pLogger->warning(Logging::FLOWERCARE, "Failed to set " + m_pBLEInterface->getIdentifier() + " to real time mode");
    return false;
  }

  // Read real time data
  m_pLogger->info(Logging::FLOWERCARE, "Getting real time data from " + m_pBLEInterface->getIdentifier());
  std::string response = m_pBLEInterface->getValue(FLOWERCARE_HANDLE_REALTIMEDATA);

  // Check if response has right length
  if(response.length() != FLOWERCARE_RESPONSE_REALTIMEDATA) {
    m_pLogger->warning(Logging::FLOWERCARE, "Device " + m_pBLEInterface->getIdentifier() + " response is unexpected");
    return false;
  }

  // Extract soil data
  m_realTimeData->setTemperature((response.at(0) | response.at(1) >> 8));
  m_realTimeData->setBrightness((response.at(3) | response.at(4) >> 8) | (response.at(5) >> 16 | response.at(6) >> 24));
  m_realTimeData->setMoisture(response.at(7));
  m_realTimeData->setConductivity((response.at(8) | response.at(9) >> 8));
  m_pLogger->info(Logging::FLOWERCARE, "Real time data is " + m_realTimeData->toString());

  m_realTimeDataLastTime = millis();
  return true;
}
