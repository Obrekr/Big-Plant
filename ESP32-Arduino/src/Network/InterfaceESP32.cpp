#include "InterfaceESP32.h"

namespace Network {
  const char* InterfaceESP32::p_stringAPStarting = "Starting access point";
  const char* InterfaceESP32::p_stringAPStart = "Started access point with %s";
  const char* InterfaceESP32::p_stringAPStartFail = "Failed to start access point";
  const char* InterfaceESP32::p_stringAPHostnameSet = "Set custom hostname for access point";
  const char* InterfaceESP32::p_stringAPIPSet = "Set custom ip address for access point";
  const char* InterfaceESP32::p_stringAPStop = "Stopped access point";
  const char* InterfaceESP32::p_stringAPStopFail = "Failed to stop access point";
  const char* InterfaceESP32::p_stringSSIDPassphraseWrong = "SSID or passphrase is not valid";
  
  InterfaceESP32::InterfaceESP32(Configuration* p_accesspoint, Configuration* p_station, Logging::Interface* p_logger)
      : m_pAccesspoint(p_accesspoint), m_pStation(p_station), m_pLogger(p_logger)
  {}
  
  bool InterfaceESP32::APstart() {
    bool ret = true;
    m_pLogger->info(p_stringAPStarting);
    
    // Check if SSID and passphrase is set and start access point
    if(!(m_pAccesspoint->hasSSID() && m_pAccesspoint->hasPassphrase()))
    {
      m_pLogger->error(p_stringSSIDPassphraseWrong);
      goto fail;
    }
    ret &= WiFi.softAP(m_pAccesspoint->getSSID(), m_pAccesspoint->getPassphrase());
    
    // Check if hostname should be set
    if(m_pAccesspoint->hasHostname()) {
      ret &= WiFi.softAPsetHostname(m_pAccesspoint->getHostname());
      m_pLogger->info(p_stringAPHostnameSet);
    }
    
    // Check if specific IP should be set
    if(m_pAccesspoint->hasIP() && m_pAccesspoint->hasGateway() && m_pAccesspoint->hasSubnet())
    {
      ret &= WiFi.softAPConfig(m_pAccesspoint->getIP(), m_pAccesspoint->getGateway(), m_pAccesspoint->getSubnet());
      m_pLogger->info(p_stringAPIPSet);
    }
    
    m_pLogger->info(p_stringAPStart, m_pAccesspoint->toString());
    return ret;
  fail:
    m_pLogger->error(p_stringAPStartFail);
    return false;
  }
    
  bool InterfaceESP32::APstatus() {
    return (bool) (WiFi.getStatusBits() & AP_STARTED_BIT);
  }
  
  bool InterfaceESP32::APstop() {
    bool ret = true;
    
    if(!WiFi.softAPdisconnect(true)) {
      ret = false;
      m_pLogger->error(p_stringAPStopFail);
    }
    
    m_pLogger->info(p_stringAPStop);
    return ret;
  }
}