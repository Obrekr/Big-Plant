#include "InterfaceESP32.h"

namespace Provisioning {
  const char* InterfaceESP32::m_pStringNVSFail = "Failed to open prefrences on NVS";
  const char* InterfaceESP32::m_pStringWiFiAPLoading = "Loading WiFi AP settings";
  const char* InterfaceESP32::m_pStringWiFiAPSuccess = "Successfully loaded Wifi AP settings: %s";
  const char* InterfaceESP32::m_pStringWiFiAPDefaultSuccess = "Generated and saved default values for WiFi AP settings: %s";
  const char* InterfaceESP32::m_pStringWiFiAPFail = "Got no valid WiFi AP settings";
  
  InterfaceESP32::InterfaceESP32(Logging::Interface* p_logger) : m_pLogger(p_logger) {
    m_hasConfigWifiAP = false;
    m_pConfigWiFiAP = new ConfigWiFiAP(&preferences);
    
    if(!preferences.begin("bigplant", false)) {
      m_pLogger->error(m_pStringNVSFail);
      return;
    }
    
    // Load configWiFiAP
    m_pLogger->info(m_pStringWiFiAPLoading);
    m_hasConfigWifiAP = m_pConfigWiFiAP->load();
    if(m_hasConfigWifiAP) {
      m_pLogger->info(m_pStringWiFiAPSuccess, m_pConfigWiFiAP->get()->toString());
    } else {
      m_hasConfigWifiAP = m_pConfigWiFiAP->generateDefault();
      if(m_hasConfigWifiAP) {
        m_pLogger->warning(m_pStringWiFiAPDefaultSuccess, m_pConfigWiFiAP->get()->toString());
      } else {
         m_pLogger->error(m_pStringWiFiAPFail);
      }
    }
  }
  InterfaceESP32::~InterfaceESP32() {
    preferences.end();
    delete m_pConfigWiFiAP;
  }
  
  bool InterfaceESP32::startWebServer() {
    return true;
  }
  
  bool InterfaceESP32::stopWebServer() {
    return true;
  }
  
  bool InterfaceESP32::hasConfigWiFiAP() {
    return m_pConfigWiFiAP != NULL && m_hasConfigWifiAP;
  }
  
  Network::Configuration* InterfaceESP32::getConfigWiFiAP() {
    return m_pConfigWiFiAP->get();
  }
  
  bool InterfaceESP32::saveConfigWiFiAP() {
    return m_pConfigWiFiAP->save();
  }
}