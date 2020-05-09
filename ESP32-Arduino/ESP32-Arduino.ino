#include "BLEDevice.h"
#include "src/Logging/LogManager.h"
#include "src/Logging/Interface.h"
#include "src/Logging/Output.h"
#include "src/Logging/OutputArduinoSerial.h"
#include "src/BLE/Interface.h"
#include "src/BLE/InterfaceESP32.h"
#include "src/Sensor/FlowerCare/FlowerCare.h"

#define SW_NAME "Pflanzen Monitor"
#define SW_VERSION "0.1"

#define SENSOR_MAC "c4:7c:8d:6a:55:df"

// Setup logging
Logging::LogManager* logManager = new Logging::LogManager();
Logging::Output* loggingOutputArduinoSerial = new Logging::OutputArduinoSerial(115200);
Logging::Interface* loggingBLE = new Logging::Interface(Logging::LOCAL0, Logging::DEBUG, "BLE", logManager);
Logging::Interface* loggingFlowerCare = new Logging::Interface(Logging::LOCAL0, Logging::DEBUG, "FlowerCare", logManager);

BLEAddress* bleAddress = new BLEAddress(SENSOR_MAC);
BLEClient* bleClient;
BLE::Interface* bleInterface;
Sensor::FlowerCare::FlowerCare* sensor;

void setup() {
  logManager->addOutput(loggingOutputArduinoSerial);
  //logger.startup(SW_NAME);
  //logger.startup(std::string("Version: ") + SW_VERSION);
  BLEDevice::init("");
  bleClient = BLEDevice::createClient();
  bleInterface = new BLE::InterfaceESP32(bleClient, bleAddress, loggingBLE);
  sensor = new Sensor::FlowerCare::FlowerCare(bleInterface, loggingFlowerCare);
}

void loop() {
  
}
