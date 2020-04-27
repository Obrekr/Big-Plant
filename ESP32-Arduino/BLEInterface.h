#ifndef BLEInterface_h
#define BLEInterface_h

#include "BLEClient.h"
#include "Logging.h"

class BLEInterface {
  public:
    virtual bool connect();
    virtual bool isConnected();
    virtual void disconnect();
    virtual bool hasHandle(uint16_t handle);
    virtual std::string getIdentifier();
    virtual std::string getValue(uint16_t handle);
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
