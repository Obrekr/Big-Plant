#include "Configuration.h"

namespace Network {
  const unsigned int Configuration::m_maxLengthSSID = 31;
  const unsigned int Configuration::m_maxLengthPassphrase = 64;
  const unsigned int Configuration::m_maxLengthHostname = 63;
  
  Configuration::Configuration()
      : m_pSsid(NULL), m_pPassphrase(NULL), m_pHostname(NULL), m_ip(0),  m_gateway(0),  m_subnet(0)
  {
    m_pStringRepresentation = new char();
  }
  
  Configuration::Configuration(const char* p_ssid, const char* p_passphrase)
        : m_pSsid(p_ssid), m_pPassphrase(p_passphrase), m_pHostname(NULL), 
          m_ip(0),  m_gateway(0),  m_subnet(0)
  {
    m_pStringRepresentation = new char();
  }
  
  Configuration::Configuration(const char* p_ssid, const char* p_passphrase, const char* p_hostname)
      : m_pSsid(p_ssid), m_pPassphrase(p_passphrase), m_pHostname(p_hostname), 
        m_ip(0),  m_gateway(0),  m_subnet(0)
  {
    m_pStringRepresentation = new char();
  }
  
  Configuration::Configuration(const char* p_ssid, const char* p_passphrase, const char* p_hostname, uint32_t ip, uint32_t gateway, uint32_t subnet)
      : m_pSsid(p_ssid), m_pPassphrase(p_passphrase), m_pHostname(p_hostname), 
        m_ip(ip),  m_gateway(gateway),  m_subnet(subnet)
  {
    m_pStringRepresentation = new char();
  }
  
  Configuration::~Configuration() {
    delete[] m_pStringRepresentation;
  }
  
  bool Configuration::hasSSID() {
    if(m_pSsid == NULL || strlen(m_pSsid) == 0 || strlen(m_pSsid) > m_maxLengthSSID) {
      return false;
    }
    
    for(uint8_t i = 0; i < strlen(m_pSsid); i++) {
      if(m_pSsid[i] < 32 || m_pSsid[i] > 126) {
        return false;
      }
    }
    
    return true;
  }
  
  bool Configuration::hasPassphrase() {
    return m_pPassphrase!= NULL && strlen(m_pPassphrase) >= 8 && strlen(m_pPassphrase) <= m_maxLengthPassphrase;
  }
  
  bool Configuration::hasHostname() {
    return m_pHostname != NULL && strlen(m_pHostname) != 0 && strlen(m_pHostname) <= m_maxLengthHostname;
  }
  
  bool Configuration::hasIP() {
    return m_ip != 0;
  }
  
  bool Configuration::hasGateway() {
    return m_gateway != 0;
  }
  
  bool Configuration::hasSubnet() {
    return m_subnet != 0;
  }
  
  char* Configuration::toString() {
    if(hasSSID() && hasHostname()) {
      format("SSID='%s' Hostname='%s' IP=%u.%u.%u.%u Gateway=%u.%u.%u.%u Subnet=%u.%u.%u.%u",
          m_pSsid,
          m_pHostname,
          m_ip & 0xFF, (m_ip >> 8) & 0xFF, (m_ip >> 16) & 0xFF, (m_ip >> 24) & 0xFF,
          m_gateway & 0xFF,(m_gateway >> 8) & 0xFF, (m_gateway >> 16) & 0xFF, (m_gateway >> 24) & 0xFF, 
          m_subnet & 0xFF, (m_subnet >> 8) & 0xFF, (m_subnet >> 16) & 0xFF, (m_subnet >> 24) & 0xFF);
    } else if (hasSSID() && !hasHostname()) {
      format("SSID='%s' IP=%u.%u.%u.%u Gateway=%u.%u.%u.%u Subnet=%u.%u.%u.%u",
          m_pSsid,
          m_ip & 0xFF, (m_ip >> 8) & 0xFF, (m_ip >> 16) & 0xFF, (m_ip >> 24) & 0xFF,
          m_gateway & 0xFF,(m_gateway >> 8) & 0xFF, (m_gateway >> 16) & 0xFF, (m_gateway >> 24) & 0xFF,
          m_subnet & 0xFF, (m_subnet >> 8) & 0xFF, (m_subnet >> 16) & 0xFF, (m_subnet >> 24) & 0xFF);
    } else if (!hasSSID() && hasHostname()) {
      format("Hostname='%s' IP=%u.%u.%u.%u Gateway=%u.%u.%u.%u Subnet=%u.%u.%u.%u",
          m_pHostname,
          m_ip & 0xFF, (m_ip >> 8) & 0xFF, (m_ip >> 16) & 0xFF, (m_ip >> 24) & 0xFF,
          m_gateway & 0xFF,(m_gateway >> 8) & 0xFF, (m_gateway >> 16) & 0xFF, (m_gateway >> 24) & 0xFF,
          m_subnet & 0xFF, (m_subnet >> 8) & 0xFF, (m_subnet >> 16) & 0xFF, (m_subnet >> 24) & 0xFF);
    } else {
      format("IP=%u.%u.%u.%u Gateway=%u.%u.%u.%u Subnet=%u.%u.%u.%u", 
          m_ip & 0xFF, (m_ip >> 8) & 0xFF, (m_ip >> 16) & 0xFF, (m_ip >> 24) & 0xFF,
          m_gateway & 0xFF,(m_gateway >> 8) & 0xFF, (m_gateway >> 16) & 0xFF, (m_gateway >> 24) & 0xFF,
          m_subnet & 0xFF, (m_subnet >> 8) & 0xFF, (m_subnet >> 16) & 0xFF, (m_subnet >> 24) & 0xFF);
    }
    
    return m_pStringRepresentation;
  }
  
  void Configuration::format(const char* p_formatString, ...) {
    va_list arguments;
    va_start(arguments, p_formatString);
  
    int finalLength = vsnprintf(NULL, 0, p_formatString, arguments);
    if(finalLength > 0) {
      delete[] m_pStringRepresentation;
      m_pStringRepresentation = new char[finalLength + 1];
      vsnprintf(m_pStringRepresentation, finalLength + 1, p_formatString, arguments);
    }
  }
}