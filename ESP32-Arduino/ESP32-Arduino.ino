#include "BLEDevice.h"
#include "src/Logging/LogManager.h"
#include "src/Logging/Interface.h"
#include "src/Logging/Output.h"
#include "src/Logging/OutputArduinoSerial.h"
#include "src/Network/Interface.h"
#include "src/Network/InterfaceESP32.h"
#include "src/BLE/Interface.h"
#include "src/BLE/InterfaceESP32.h"
#include "src/Sensor/FlowerCare/FlowerCare.h"

#define SW_NAME "Pflanzen Monitor"
#define SW_VERSION "0.1"

#define SENSOR_MAC "c4:7c:8d:6a:55:df"

// Setup logging
Logging::LogManager* logManager = new Logging::LogManager();
Logging::Output* loggingOutputArduinoSerial = new Logging::OutputArduinoSerial(115200);
Logging::Interface* loggingKernel = new Logging::Interface(Logging::KERN, Logging::DEBUG, "Kernel", logManager);
Logging::Interface* loggingNetwork = new Logging::Interface(Logging::LOCAL0, Logging::DEBUG, "Network", logManager);
Logging::Interface* loggingBLE = new Logging::Interface(Logging::LOCAL0, Logging::DEBUG, "BLE", logManager);
Logging::Interface* loggingFlowerCare = new Logging::Interface(Logging::LOCAL0, Logging::DEBUG, "FlowerCare", logManager);

// Setup WiFi
Network::Configuration* networkingAPConfig = new Network::Configuration("Hello World!", "geheimgeheimgeheim");
Network::Configuration* networkingSTAConfig = new Network::Configuration();
Network::Interface* networking = new Network::InterfaceESP32(networkingAPConfig, networkingSTAConfig, loggingNetwork);

BLEAddress* bleAddress = new BLEAddress(SENSOR_MAC);
BLEClient* bleClient;
BLE::Interface* bleInterface;
Sensor::FlowerCare::FlowerCare* sensor;

void setup() {
  logManager->addOutput(loggingOutputArduinoSerial);
  loggingKernel->info(SW_NAME);
  loggingKernel->info("Version: %s", SW_VERSION);
  BLEDevice::init("");
  bleClient = BLEDevice::createClient();
  bleInterface = new BLE::InterfaceESP32(bleClient, bleAddress, loggingBLE);
  //sensor = new Sensor::FlowerCare::FlowerCare(bleInterface, loggingFlowerCare);

  networking->APstart();
}

void loop() {
  // Check if WiFi Credentials are set & reachable (try multiple times)
    // If not: start AP
    
}
