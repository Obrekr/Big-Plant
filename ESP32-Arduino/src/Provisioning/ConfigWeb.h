#ifndef Provisioning_ConfigWeb_h
#define Provisioning_ConfigWeb_h

#include "Storage.h"
#include "ConfigWiFiAP.h"
#include "../Utility/Utility.h"
#include "../Webserver/Configuration.h"
#include "../Webserver/Interface.h"

namespace Provisioning {
  class ConfigWeb {
    public:
      ConfigWeb(Storage* p_storage, ConfigWiFiAP* p_configWiFiAP, Webserver::Interface* p_webserver);
      ~ConfigWeb();
      
      bool load();
      bool save();
      bool generateDefault();
      
      Webserver::Configuration* get();
      
    private:
      Storage* m_pStorage;
      ConfigWiFiAP* m_pConfigWiFiAP;
      Webserver::Interface* m_pWebserver;
      Webserver::Configuration* m_pWebServerConfig;
      unsigned char* m_pKey;
      unsigned char* m_pCACertificate;
  };
}
#endif