#ifndef Provisioning_Interface_h
#define Provisioning_Interface_h

#include "Storage.h"
#include "ConfigWiFiAP.h"
#include "ConfigWeb.h"
#include "../Logging/Interface.h"
#include "../Network/Configuration.h"
#include "../Webserver/Interface.h"

namespace Provisioning {
  class Interface {
    public:
      Interface(Storage* p_storage, Webserver::Interface* p_webserver, Logging::Interface* p_logger);
      ~Interface();
      
      bool hasConfigWiFiAP() {return m_pConfigWiFiAP != NULL && m_hasConfigWifiAP;}
      Network::Configuration* getConfigWiFiAP() {return m_pConfigWiFiAP->get();}
      bool saveConfigWiFiAP() {return m_pConfigWiFiAP->save();}
      
      bool hasConfigWeb() {return m_pConfigWeb != NULL && m_hasConfigWeb;}
      Webserver::Configuration* getConfigWeb() {return m_pConfigWeb->get();}
      bool saveConfig() {return m_pConfigWeb->save();}
      
    private:
      Storage* m_pStorage;
      Webserver::Interface* m_pWebserver;
      Logging::Interface* m_pLogger;
      ConfigWiFiAP* m_pConfigWiFiAP;
      bool m_hasConfigWifiAP;
      ConfigWeb* m_pConfigWeb;
      bool m_hasConfigWeb;
      static const char* m_pStringWiFiAPLoading;
      static const char* m_pStringWiFiAPSuccess;
      static const char* m_pStringWiFiAPDefaultSuccess;
      static const char* m_pStringWiFiAPFail;
      static const char* m_pStringWebLoading;
      static const char* m_pStringWebSuccess;
      static const char* m_pStringWebDefaultSuccess;
      static const char* m_pStringWebFail;
  };
}
#endif
