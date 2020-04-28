#ifndef BLEInterface_h
#define BLEInterface_h

#include "BLEClient.h"
#include "Logging.h"

class BLEInterface {
  public:
    virtual bool connect() = 0;
    virtual bool isConnected() = 0;
    virtual void disconnect() = 0;
    virtual bool hasHandle(uint16_t handle) = 0;
    virtual std::string getIdentifier() = 0;
    virtual std::string getValue(uint16_t handle) = 0;
    virtual bool setValue(uint16_t handel, std::string data) = 0;
};

class BLEInterfaceESP32 : public BLEInterface {
  public:
    BLEInterfaceESP32(BLEClient* p_bleClient, BLEAddress* p_bleAddress, Logging* p_logger);

    bool connect();
    bool isConnected();
    void disconnect();
    bool hasHandle(uint16_t handle);
    std::string getIdentifier();
    std::string getValue(uint16_t handle);
    bool setValue(uint16_t handle, std::string data);

  private:
    BLEClient* m_pBLEClient;
    BLEAddress* m_pBLEAddress;
    Logging* m_pLogger;
    std::string m_identifier;
    std::map<std::string, BLERemoteService*>* m_pBLEServices;

    BLERemoteCharacteristic* getCharacteristicByHandle(uint16_t handle);
};

class BLEClientCallback : public BLEClientCallbacks {
  public:
    void onConnect(BLEClient* pclient) {}
    void onDisconnect(BLEClient* pclient) {}
};
#endif
