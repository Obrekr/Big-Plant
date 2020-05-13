#include "Interface.h"

namespace Provisioning {
  const char* Interface::m_pStringWiFiAPLoading = "Loading WiFi AP settings";
  const char* Interface::m_pStringWiFiAPSuccess = "Successfully loaded Wifi AP settings: %s";
  const char* Interface::m_pStringWiFiAPDefaultSuccess = "Generated and saved default values for WiFi AP settings: %s";
  const char* Interface::m_pStringWiFiAPFail = "Got no valid WiFi AP settings";
  
  Interface::Interface(Storage* p_storage, Logging::Interface* p_logger) : m_pStorage(p_storage), m_pLogger(p_logger) {
    m_hasConfigWifiAP = false;
    m_pConfigWiFiAP = new ConfigWiFiAP(m_pStorage);
    
    // Load configWiFiAP: SSID and passphrase have to be valid for wifi access point, else use default settings
    m_pLogger->info(m_pStringWiFiAPLoading);
    if(m_pConfigWiFiAP->load() && m_pConfigWiFiAP->get()->hasSSID() && m_pConfigWiFiAP->get()->hasPassphrase()) {
      m_hasConfigWifiAP = true;
      m_pLogger->info(m_pStringWiFiAPSuccess, m_pConfigWiFiAP->get()->toString());
    } else {
      if(m_pConfigWiFiAP->generateDefault()) {
        m_hasConfigWifiAP = true;
        m_pLogger->warning(m_pStringWiFiAPDefaultSuccess, m_pConfigWiFiAP->get()->toString());
      } else {
        m_hasConfigWifiAP = false;
         m_pLogger->error(m_pStringWiFiAPFail);
      }
    }
  }
  Interface::~Interface() {
    delete m_pConfigWiFiAP;
  }
  
  bool Interface::startWebServer() {
    return true;
  }
  
  bool Interface::stopWebServer() {
    return true;
  }
}