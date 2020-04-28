#include "BLEDevice.h"
#include "Logging.h"
#include "FlowerCare.h"
#include "BLEInterface.h"

#define SW_NAME "Pflanzen Monitor"
#define SW_VERSION "0.1"

#define SENSOR_MAC "c4:7c:8d:6a:55:df"

Logging logger = Logging(Logging::DEBUG);
BLEAddress bleAddress = BLEAddress(SENSOR_MAC);
BLEClient* bleClient;
BLEInterface* bleInterface;
FlowerCare* sensor;

void setup() {
  logger.startup(SW_NAME);
  logger.startup(std::string("Version: ") + SW_VERSION);
  BLEDevice::init("");
  bleClient = BLEDevice::createClient();
  bleInterface = new BLEInterfaceESP32(bleClient, &bleAddress, &logger);
  sensor = new FlowerCare(bleInterface, &logger);
}

void loop() {
  
}
