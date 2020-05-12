#ifndef Provisioning_InterfaceESP32_h
#define Provisioning_InterfaceESP32_h

#include "Interface.h"
#include <Preferences.h>
#include "ConfigWiFiAP.h"
#include "../Logging/Interface.h"

namespace Provisioning {
  class InterfaceESP32 : public Interface {
    public:
      InterfaceESP32(Logging::Interface* p_logger);
      ~InterfaceESP32();
      
      bool startWebServer();
      bool stopWebServer();
      
      bool hasConfigWiFiAP();
      Network::Configuration* getConfigWiFiAP();
      bool saveConfigWiFiAP();
      
    private:
      Logging::Interface* m_pLogger;
      Preferences preferences;
      ConfigWiFiAP* m_pConfigWiFiAP;
      bool m_hasConfigWifiAP;
      static const char* m_pStringNVSFail;
      static const char* m_pStringWiFiAPLoading;
      static const char* m_pStringWiFiAPSuccess;
      static const char* m_pStringWiFiAPDefaultSuccess;
      static const char* m_pStringWiFiAPFail;
  };
}
#endif
