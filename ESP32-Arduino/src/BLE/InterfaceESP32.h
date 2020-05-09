#ifndef BLE_InterfaceESP32_h
#define BLE_InterfaceESP32_h

#include "BLEClient.h"
#include "esp32-hal.h"
#include "Interface.h"
#include "LogMessages.h"
#include "../Logging/Interface.h"

namespace BLE {
  class InterfaceESP32 : public Interface {
    public:
      InterfaceESP32(BLEClient* p_bleClient, BLEAddress* p_bleAddress, Logging::Interface* p_logger);
      ~InterfaceESP32();

      bool connect();
      bool isConnected();
      void disconnect();
      bool hasHandle(uint16_t handle);
      char* getIdentifier();
      std::string getValue(uint16_t handle);
      bool setValue(uint16_t handle, std::string data);

    private:
      BLEClient* m_pBLEClient;
      BLEAddress* m_pBLEAddress;
      Logging::Interface* m_pLogger;
    
      char* m_identifier;
      std::map<std::string, BLERemoteService*>* m_pBLEServices;

      BLERemoteCharacteristic* getCharacteristicByHandle(uint16_t handle);
  };

  class ClientCallback : public BLEClientCallbacks {
    public:
      void onConnect(BLEClient* pclient) {}
      void onDisconnect(BLEClient* pclient) {}
  };
}
#endif
