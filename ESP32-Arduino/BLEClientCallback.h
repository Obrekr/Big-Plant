#ifndef BLEClientCallback_h
#define BLEClientCallback_h

#include "BLEClient.h"

class BLEClientCallback : public BLEClientCallbacks {
  public:
    void onConnect(BLEClient* pclient);
    void onDisconnect(BLEClient* pclient);
};
#endif
