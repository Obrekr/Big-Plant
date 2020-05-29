#ifndef Webserver_InterfaceESP32_h
#define Webserver_InterfaceESP32_h

#include "Interface.h"
#include "Configuration.h"
#include "../Logging/Interface.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp32-hal.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <mbedtls/pk.h>
#include <mbedtls/x509_crt.h>

namespace Webserver {
  class InterfaceESP32 : public Interface {
    public:
      InterfaceESP32(Logging::Interface* p_logger);
    
      bool start();
      bool stop();
    
      bool generateSelfSignedCertificate(Configuration* p_configuration, unsigned int keyLength, const char* p_domainName,
                                               const char* p_validFrom, const char* p_validUntil);
                                               
     static const char* m_pStringSelfSignedStart;
     static const char* m_pStringKeygenStart;
     static const char* m_pStringKeygenRNG;
     static const char* m_pStringKeygenSetup;
     static const char* m_pStringKeygenGenerate;
     static const char* m_pStringKeygenSave;
     static const char* m_pStringCertgenStart;
     static const char* m_pStringCertgenName;
     static const char* m_pStringCertgenValidFromUntil;
     static const char* m_pStringCertgenSerialStart;
     static const char* m_pStringCertgenSerial;
     static const char* m_pStringCertgenSave;
     static const char* m_pStringSelfSignedSuccess;
    private:
      Logging::Interface* m_pLogger;
  };
  
  typedef struct {
    Logging::Interface* p_logger;
    Configuration* p_configuration;
    unsigned int keyLength;
    const char* p_domainName;
    const char* p_validFrom;
    const char* p_validUntil;
    volatile bool completed;
    bool success;
  } CertificateSettings;
  
  void taskSelfSignedCertificate(void* parameter);
}
#endif