#ifndef Provisioning_ConfigWiFiAP_h
#define Provisioning_ConfigWiFiAP_h

#include "Storage.h"
#include "../Logging/Interface.h"
#include "../Network/Configuration.h"

namespace Provisioning {
  class ConfigWiFiAP {
    public:
      ConfigWiFiAP(Storage* p_storage);
      ~ConfigWiFiAP();
      
      bool load();
      bool save();
      bool generateDefault();
      
      Network::Configuration* get();
      
    private:
      Storage* m_pStorage;
      Network::Configuration* m_pNetworkConfig;
      char* m_pSSID;
      char* m_pPassphrase;
      char* m_pHostname;
  };
}
#endif