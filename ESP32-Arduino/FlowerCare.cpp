#include "FlowerCare.h"

FlowerCare::FlowerCare(BLEClient* bleclient, BLEAddress* bleaddress, Logging* logger) {
  this->client = bleclient;
  this->address = bleaddress;
  this->logger = logger;

  // Try connecting to device
  if(!this->connect()) {
    this->logger->warning(Logging::FLOWERCARE, "Could not connect");
    this->logger->warning(Logging::FLOWERCARE, "Is " + this->address->toString() + " the correct device?");
    this->logger->error(Logging::FLOWERCARE, "Initialization failed for " + this->address->toString());
    return;
  }

  // Check if all expected BLE handles are present
  if(!(this->hasHandle(FLOWERCARE_HANDLE_BATTERYFIRMWARE))) {
    this->logger->warning(Logging::FLOWERCARE, "Some BLE handles are not available");
    this->logger->warning(Logging::FLOWERCARE, "Is " + this->address->toString() + " the correct device?");
    this->logger->error(Logging::FLOWERCARE, "Initialization failed for " + this->address->toString());
    return;
  }
  
  // Get device firmware version
  if(!this->loadBatteryAndFirmware()) {
    this->logger->error(Logging::FLOWERCARE, "Initialization failed for " + this->address->toString());
    return;
  }

  this->disconnect();
  this->initSuccess = true;
  this->logger->info(Logging::FLOWERCARE, "Successfully initialized " + this->address->toString());
}

bool FlowerCare::connect() {
  this->logger->info(Logging::FLOWERCARE, "Connecting to " + this->address->toString());
  this->client->connect(*this->address);
  if(!this->isConnected()) {
    this->logger->error(Logging::BLE, "Failed to connect to " + this->address->toString());
    return false;
  }
  this->services = this->client->getServices();
  return true;
}

bool FlowerCare::isConnected() {
  if(this->client->isConnected() && this->client->getPeerAddress().equals(*this->address)) {
    return true;
  }
  this->logger->info(Logging::BLE, "Not connected to " + this->address->toString());
  return false;
}

void FlowerCare::disconnect() {
  // Remove pointers to BLE services, they will be invalid after disconnect
  this->services->clear();
  
  // Disconnect BLE
  this->client->disconnect();
  this->logger->info(Logging::FLOWERCARE, "Disconnected from " + this->address->toString());
}

BLERemoteCharacteristic* FlowerCare::getCharacteristicByHandle(uint16_t handle) {
  char stringConvert[8];
  itoa(handle, stringConvert, 10);

  for (auto const& entry : *this->services) {
    std::map<uint16_t, BLERemoteCharacteristic*>::iterator it = entry.second->getCharacteristicsByHandle()->find(handle);
    if(it != entry.second->getCharacteristicsByHandle()->end()) {
      this->logger->debug(Logging::BLE, "Found characerisitc for handle " + std::string(stringConvert));
      return it->second;
    }
  }
  this->logger->warning(Logging::BLE, "Device " + this->address->toString() + " has no handle " + std::string(stringConvert));
  return NULL;
}

bool FlowerCare::hasHandle(uint16_t handle) {
  return this->getCharacteristicByHandle(handle) != NULL;
}

bool FlowerCare::loadBatteryAndFirmware() {
  char count[8];
  
  // Check if BLE client is connected
  if(!this->client->isConnected()) {
    this->logger->warning(Logging::FLOWERCARE, "Device " + this->address->toString() + "is currently unavailabel");
    return false;
  }

  // Read battery level and firmware version from BLE client
  BLERemoteCharacteristic* characteristic = this->getCharacteristicByHandle(FLOWERCARE_HANDLE_BATTERYFIRMWARE);
  if(characteristic == NULL) {
    this->logger->warning(Logging::FLOWERCARE, "Cannot access battery level and firmware information");
    this->logger->warning(Logging::FLOWERCARE, "Is " + this->address->toString() + " the correct device?");
    return false;
  }
  std::string response = characteristic->readValue();
  this->logger->info(Logging::FLOWERCARE, "Getting battery level and firmware version from " + this->address->toString());  
  itoa(response.length(), count, 10);
  this->logger->debug(Logging::BLE, "Received " + std::string(count) + " bytes: " + response);

  // Check if response is plausible: fixed byte count received and magic byte 1 should be 0x13 or 0x28 or 0x2A
  if(!(response.length() == FLOWERCARE_RESPONSE_BATTERYFIRMWARE && (response.at(1) == 0x13 || response.at(1) == 0x28 || response.at(1) == 0x2A))) {
    this->logger->warning(Logging::FLOWERCARE, "Device " + this->address->toString() + " response is unexpected");
    return false;
  }

  // Extract battery level
  this->batteryLevel = response.at(0);
  if(this->batteryLevel > 100) {
    this->logger->warning(Logging::FLOWERCARE, "Device " + this->address->toString() + " reported invalid battery level");
    return false;
  }
  itoa(this->batteryLevel, count, 10);
  this->logger->info(Logging::FLOWERCARE, "Battery Level is " + std::string(count) + "%");

  // Extract firmware version
  this->firmwareVersion = response.substr(2, std::string::npos);
  this->logger->info(Logging::FLOWERCARE, "Firmware version is " + this->firmwareVersion);

  return true;
}
