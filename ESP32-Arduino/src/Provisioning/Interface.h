#ifndef Provisioning_Interface_h
#define Provisioning_Interface_h

#include "Storage.h"
#include "ConfigWiFiAP.h"
#include "../Logging/Interface.h"
#include "../Network/Configuration.h"

namespace Provisioning {
  class Interface {
    public:
      Interface(Storage* p_storage, Logging::Interface* p_logger);
      ~Interface();
      
      bool startWebServer();
      bool stopWebServer();
      
      bool hasConfigWiFiAP() {return m_pConfigWiFiAP != NULL && m_hasConfigWifiAP;}
      Network::Configuration* getConfigWiFiAP() {return m_pConfigWiFiAP->get();}
      bool saveConfigWiFiAP() {return m_pConfigWiFiAP->save();}
      
    private:
      Storage* m_pStorage;
      Logging::Interface* m_pLogger;
      ConfigWiFiAP* m_pConfigWiFiAP;
      bool m_hasConfigWifiAP;
      static const char* m_pStringWiFiAPLoading;
      static const char* m_pStringWiFiAPSuccess;
      static const char* m_pStringWiFiAPDefaultSuccess;
      static const char* m_pStringWiFiAPFail;
  };
}
#endif
