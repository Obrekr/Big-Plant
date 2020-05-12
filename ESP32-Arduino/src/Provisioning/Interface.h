#ifndef Provisioning_Interface_h
#define Provisioning_Interface_h

#include "../Network/Configuration.h"

namespace Provisioning {
  class Interface {
    public:
      virtual bool startWebServer() = 0;
      virtual bool stopWebServer() = 0;
      
      virtual bool hasConfigWiFiAP() = 0;
      virtual Network::Configuration* getConfigWiFiAP() = 0;
      virtual bool saveConfigWiFiAP() = 0;
  };
}
#endif
