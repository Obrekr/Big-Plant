#include "Esp.h"
#include "BLEDevice.h"
#include "src/Logging/LogManager.h"
#include "src/Logging/Interface.h"
#include "src/Logging/OutputArduinoSerial.h"
#include "src/Network/InterfaceESP32.h"
#include "src/Provisioning/StorageESP32.h"
#include "src/Provisioning/Interface.h"
#include "src/Webserver/InterfaceESP32.h"
#include "src/BLE/InterfaceESP32.h"
#include "src/Sensor/FlowerCare/FlowerCare.h"

#include "esp_heap_caps.h"

#define SW_NAME "Pflanzen Monitor"
#define SW_VERSION "0.1"

#define SENSOR_MAC "c4:7c:8d:6a:55:df"

// Setup Logging
Logging::LogManager* logManager = new Logging::LogManager();
Logging::Output* loggingOutputArduinoSerial = new Logging::OutputArduinoSerial(115200);
Logging::Interface* loggingKernel = new Logging::Interface(Logging::KERN, Logging::DEBUG, "Kernel", logManager);
Logging::Interface* loggingProvisioning = new Logging::Interface(Logging::LOCAL0, Logging::DEBUG, "Provisioning", logManager);
Logging::Interface* loggingWebserver = new Logging::Interface(Logging::LOCAL0, Logging::DEBUG, "Webserver", logManager);
Logging::Interface* loggingNetwork = new Logging::Interface(Logging::LOCAL0, Logging::DEBUG, "Network", logManager);
Logging::Interface* loggingBLE = new Logging::Interface(Logging::LOCAL0, Logging::DEBUG, "BLE", logManager);
Logging::Interface* loggingFlowerCare = new Logging::Interface(Logging::LOCAL0, Logging::DEBUG, "FlowerCare", logManager);

// Setup Provisioning
Provisioning::Storage* provisioningStorage;
Provisioning::Interface* provisioning;

// Setup WiFi
Network::Configuration* networkingSTAConfig = new Network::Configuration();
Network::Interface* networking;

//Setup Webserver
Webserver::Interface* webserver = new Webserver::InterfaceESP32(loggingWebserver);

/*BLEAddress* bleAddress = new BLEAddress(SENSOR_MAC);
BLEClient* bleClient;
BLE::Interface* bleInterface;
Sensor::FlowerCare::FlowerCare* sensor;*/

void setup() {
  // Add logging outputs and print version information
  logManager->addOutput(loggingOutputArduinoSerial);
  loggingKernel->info(SW_NAME);
  loggingKernel->info("Version: %s", SW_VERSION);

  // Load settings from non volatile storage
  provisioningStorage = new Provisioning::StorageESP32(loggingProvisioning);
  provisioning = new Provisioning::Interface(provisioningStorage, webserver, loggingProvisioning);

  // Reset stored config to defaults
  //provisioning->resetConfigWiFiAP();
  //provisioning->resetConfigWeb();
  
  // Bring up networking or reboot
  if(provisioning->hasConfigWiFiAP() && provisioning->getConfigWiFiAP()->hasSSID() && provisioning->getConfigWiFiAP()->hasPassphrase()) {
    networking = new Network::InterfaceESP32(provisioning->getConfigWiFiAP(), networkingSTAConfig, loggingNetwork);
    networking->APstart();
    webserver->start(provisioning->getConfigWeb());
  } else {
    loggingKernel->emergency("Failed to get valid configuration for WiFi access point");
    loggingKernel->emergency("Rebooting...");
    ESP.restart();
  }
  
  /*heap_caps_print_heap_info(MALLOC_CAP_DEFAULT);
  loggingKernel->debug("status=%u free=%u, maxblock=%u", heap_caps_check_integrity_all(true), 
      heap_caps_get_free_size(MALLOC_CAP_DEFAULT), heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT));*/
  
  // BLE
  //BLEDevice::init("");
  //bleClient = BLEDevice::createClient();
  //bleInterface = new BLE::InterfaceESP32(bleClient, bleAddress, loggingBLE);
  //sensor = new Sensor::FlowerCare::FlowerCare(bleInterface, loggingFlowerCare);
}

void loop() {
  // Check if WiFi Credentials are set & reachable (try multiple times)
    // If not: start AP
    
}
