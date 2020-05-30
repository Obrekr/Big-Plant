#include "Interface.h"

namespace Provisioning {
  const char* Interface::m_pStringWiFiAPLoading = "Loading WiFi AP settings";
  const char* Interface::m_pStringWiFiAPSuccess = "Successfully loaded Wifi AP settings: %s";
  const char* Interface::m_pStringWiFiAPDefaultSuccess = "Generated and saved default values for WiFi AP settings: %s";
  const char* Interface::m_pStringWiFiAPFail = "Got no valid WiFi AP settings";
  const char* Interface::m_pStringWebLoading = "Loading webserver settings";
  const char* Interface::m_pStringWebSuccess = "Successfully loaded webserver settings: %s; %s";
  const char* Interface::m_pStringWebDefaultSuccess = "Generated and saved default values for webserver settings: %s; %s";
  const char* Interface::m_pStringWebFail = "Got no valid webserver settings";
  
  Interface::Interface(Storage* p_storage, Webserver::Interface* p_webserver, Logging::Interface* p_logger) 
      : m_pStorage(p_storage), m_pWebserver(p_webserver), m_pLogger(p_logger) {
    m_hasConfigWifiAP = false;
    m_hasConfigWeb = false;
    
    // Load configWiFiAP: SSID and passphrase have to be valid for wifi access point, else use default settings
    m_pConfigWiFiAP = new ConfigWiFiAP(m_pStorage);
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
    
    // Load configWeb: Private key and certificate have to be valid, else generate a new pair
    m_pConfigWeb = new ConfigWeb(m_pStorage, m_pConfigWiFiAP, m_pWebserver);
    m_pLogger->info(m_pStringWebLoading);
    if(m_pConfigWeb->load() && m_pConfigWeb->get()->hasKey() && m_pConfigWeb->get()->hasCACertificate()) {
      m_hasConfigWeb = true;
      m_pLogger->info(m_pStringWebSuccess, m_pConfigWeb->get()->getKey(), m_pConfigWeb->get()->getCACertificate());
    } else {
      if(m_pConfigWeb->generateDefault()) {
        m_hasConfigWeb = true;
        m_pLogger->warning(m_pStringWebDefaultSuccess, m_pConfigWeb->get()->getKey(), m_pConfigWeb->get()->getCACertificate());
      } else {
        m_hasConfigWeb = false;
        m_pLogger->error(m_pStringWebFail);
      }
    }
  }
  Interface::~Interface() {
    delete m_pConfigWiFiAP;
  }
}