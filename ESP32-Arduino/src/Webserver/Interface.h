#ifndef Webserver_Interface_h
#define Webserver_Interface_h

#include "Configuration.h"

namespace Webserver {
  class Interface {
    public:
      virtual bool start(Configuration* p_configuration) = 0;
      virtual bool stop() = 0;
    
      virtual bool generateSelfSignedCertificate(Configuration* p_configuration, unsigned int keyLength, 
          const char* p_domainName, const char* p_validFrom, const char* p_validUntil) = 0;
  };
}
#endif