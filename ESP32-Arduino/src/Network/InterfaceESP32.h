#ifndef Network_InterfaceESP32_h
#define Network_InterfaceESP32_h

#include "IPAddress.h"
#include <WiFi.h>
#include "Interface.h"
#include "Configuration.h"
#include "../Logging/Interface.h"

namespace Network {
  class InterfaceESP32 : public Interface {
    public:
      InterfaceESP32(Configuration* p_accesspoint, Configuration* p_station, Logging::Interface* p_logger);
      
      bool APstart();
      bool APstatus();
      bool APstop();
    private:
      Configuration* m_pAccesspoint;
      Configuration* m_pStation;
      Logging::Interface* m_pLogger;
      static const char* p_stringAPStarting;
      static const char* p_stringAPStart;
      static const char* p_stringAPStartFail;
      static const char* p_stringAPHostnameSet;
      static const char* p_stringAPIPSet;
      static const char* p_stringAPStop;
      static const char* p_stringAPStopFail;
      static const char* p_stringSSIDPassphraseWrong;
  };
}
#endif
