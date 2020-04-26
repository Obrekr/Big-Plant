#include "FlowerCare.h"

FlowerCare::FlowerCare(BLEClient* bleclient, BLEAddress* bleaddress, Logging* logger) {
  this->client = bleclient;
  this->address = bleaddress;
  this->logger = logger;

  // Try connecting to device
  if(!this->connect()) {return;}

  // Populate all available BLE handles with their characteristics (combining all services)
  std::map<std::string, BLERemoteService*>* services = this->client->getServices();
  for (auto const& entry : *services) {
    std::map<uint16_t, BLERemoteCharacteristic*>* additionalCharaceristics = entry.second->getCharacteristicsByHandle();
    this->characteristics.insert(additionalCharaceristics->begin(), additionalCharaceristics->end());
  }

  // Check if number of characteristics is as expected
  char count[8];
  itoa(this->characteristics.size(), count, 10);
  this->logger->debug(Logging::BLE, "Device reported " + std::string(count) + " characteristics");
  if(this->characteristics.size() != FLOWERCARE_CHARACTERISTICSCOUNT) {
    this->logger->warning(Logging::FLOWERCARE, "Number of BLE characteristics doesn't match!");
    this->logger->warning(Logging::FLOWERCARE, "Is " + this->address->toString() + " the correct device?");
  }

  // Print out all gathered characteristics
  for (auto const& entry : this->characteristics) {
    this->logger->debug(Logging::BLE, entry.second->toString());
  }
  
  // Get device firmware version
  this->loadBatteryAndFirmware();

  this->deviceAvailable = true;
  // Causes Heap Corruption
  // this->disconnect();
  this->logger->info(Logging::FLOWERCARE, "Successfully initialized " + this->address->toString());
}

bool FlowerCare::connect() {
  this->logger->info(Logging::FLOWERCARE, "Connecting to " + this->address->toString());
  this->client->connect(*this->address);
  if(!this->client->isConnected()) {
    this->logger->error(Logging::BLE, "Failed to connect to " + this->address->toString());
    return false;
  }
  return true;
}

void FlowerCare::disconnect() {
  this->client->disconnect();
  this->logger->info(Logging::FLOWERCARE, "Disconnected from " + this->address->toString());
}

void FlowerCare::loadBatteryAndFirmware() {
  // Check if BLE client is connected
  if(!this->client->isConnected()) {
    this->logger->warning(Logging::FLOWERCARE, "Device " + this->address->toString() + "is currently unavailabel");
    return;
  }

  // Read battery level and firmware version from BLE client
  std::string response = this->characteristics[FLOWERCARE_HANDLE_BATTERYFIRMWARE]->readValue();
  this->logger->info(Logging::FLOWERCARE, "Getting battery level and firmware version from " + this->address->toString());  
  char count[16];
  itoa(response.length(), count, 10);
  this->logger->debug(Logging::BLE, "Received " + std::string(count) + " bytes: " + response);

  // Check if response is plausible
  if(!(response.length() == FLOWERCARE_RESPONSE_BATTERYFIRMWARE && (response.at(1) == 0x13 || response.at(1) == 0x28 || response.at(1) == 0x2A))) {
    this->logger->warning(Logging::FLOWERCARE, "Device " + this->address->toString() + " response is unexpected");
  }

  // Extract battery level
  this->batteryLevel = response.at(0);
  itoa(this->batteryLevel, count, 10);
  this->logger->info(Logging::FLOWERCARE, "Battery Level is " + std::string(count) + "%");

  // Extract firmware version
  this->firmwareVersion = response.substr(2, std::string::npos);
  this->logger->info(Logging::FLOWERCARE, "Firmware version is " + this->firmwareVersion);
}
