#ifndef BLE_Interface_h
#define BLE_Interface_h

#include <string>

namespace BLE {
  class Interface {
    public:
      virtual bool connect() = 0;
      virtual bool isConnected() = 0;
      virtual void disconnect() = 0;
      virtual bool hasHandle(uint16_t handle) = 0;
      virtual char* getIdentifier() = 0;
      virtual std::string getValue(uint16_t handle) = 0;
      virtual bool setValue(uint16_t handel, std::string data) = 0;
  };
}
#endif
