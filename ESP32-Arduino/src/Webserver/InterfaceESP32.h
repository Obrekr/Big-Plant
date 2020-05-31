#ifndef Webserver_InterfaceESP32_h
#define Webserver_InterfaceESP32_h

#include "Interface.h"
#include "Configuration.h"
#include "../Logging/Interface.h"
#include <esp_https_server.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <mbedtls/pk.h>
#include <mbedtls/x509_crt.h>
#include <mbedtls/pem.h>

namespace Webserver {
  class InterfaceESP32 : public Interface {
    public:
      InterfaceESP32(Logging::Interface* p_logger);
    
      bool start(Configuration* p_configuration);
      bool stop();
    
      bool generateSelfSignedCertificate(Configuration* p_configuration, unsigned int keyLength, const char* p_domainName,
                                               const char* p_validFrom, const char* p_validUntil);
    private:
      bool generateKey(Configuration* p_configuration, unsigned int keyLength);
      bool generateCACertificate(Configuration* p_configuration, const char* p_domainName, const char* p_validFrom, const char* p_validUntil);
      
      Logging::Interface* m_pLogger;
      httpd_handle_t m_webserver;
      static const char* m_pStringWebserverStart;
      static const char* m_pStringWebserverNoCert;
      static const char* m_pStringWebserverNoKey;
      static const char* m_pStringWebserverStartFail;
      static const char* m_pStringWebserverStartSuccess;
      static const char* m_pStringWebserverStop;
      static const char* m_pStringSelfSignedStart;
      static const char* m_pStringSelfSignedKeygenFail;
      static const char* m_pStringSelfSignedCertgenFail;
      static const char* m_pStringSelfSignedSuccess;
      static const char* m_pStringKeygenStart;
      static const char* m_pStringKeygenRNG;
      static const char* m_pStringKeygenSetup;
      static const char* m_pStringKeygenGenerate;
      static const char* m_pStringKeygenSave;
      static const char* m_pStringKeygenOutOfMemory;
      static const char* m_pStringKeygenSaveFail;
      static const char* m_pStringCertgenStart;
      static const char* m_pStringCertgenRNG;
      static const char* m_pStringCertgenPK;
      static const char* m_pStringCertgenName;
      static const char* m_pStringCertgenNameFail;
      static const char* m_pStringCertgenValidFromUntil;
      static const char* m_pStringCertgenValidFromUntilFail;
      static const char* m_pStringCertgenSerialStart;
      static const char* m_pStringCertgenSerial;
      static const char* m_pStringCertgenSave;
      static const char* m_pStringCertgenOutOfMemory;
      static const char* m_pStringCertgenSaveFail;
  };
}
#endif