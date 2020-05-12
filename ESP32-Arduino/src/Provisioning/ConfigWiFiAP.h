#ifndef Provisioning_ConfigWiFiAP_h
#define Provisioning_ConfigWiFiAP_h

#include <Preferences.h>
#include "../Logging/Interface.h"
#include "../Network/Configuration.h"

namespace Provisioning {
  class ConfigWiFiAP {
    public:
      ConfigWiFiAP(Preferences* p_preferences);
      ~ConfigWiFiAP();
      
      bool load();
      bool save();
      bool generateDefault();
      
      Network::Configuration* get();
      
    private:
      Preferences* m_pPreferences;
      Network::Configuration* m_pNetworkConfig;
      char* m_pSSID;
      char* m_pPassphrase;
      char* m_pHostname;
  };
}
#endif