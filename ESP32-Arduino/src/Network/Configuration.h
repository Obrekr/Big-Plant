#ifndef Network_Configuration_h
#define Network_Configuration_h

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <cstring>

namespace Network {
  class Configuration {
    public:
      Configuration();
      Configuration(const char* p_ssid, const char* p_passphrase);
      Configuration(const char* p_ssid, const char* p_passphrase, const char* p_hostname);
      Configuration(const char* p_ssid, const char* p_passphrase, const char* p_hostname, uint32_t ip, uint32_t gateway, uint32_t subnet);
      ~Configuration();
      
      void setSSID(const char* p_ssid) {m_pSsid = p_ssid;}
      void setPassphrase(const char* p_passphrase) {m_pPassphrase = p_passphrase;}
      void setHostname(const char* p_hostname) {m_pHostname = p_hostname;}
      void setIP(uint32_t ip) {m_ip = ip;}
      void setGateway(uint32_t gateway) {m_gateway = gateway;}
      void setSubnet(uint32_t subnet) {m_subnet = subnet;}
      
      const char* getSSID() {return m_pSsid;}
      const char* getPassphrase() {return m_pPassphrase;}
      const char* getHostname() {return m_pHostname;}
      uint32_t getIP() {return m_ip;}
      uint32_t getGateway() {return m_gateway;}
      uint32_t getSubnet() {return m_subnet;}
      
      bool hasSSID();
      bool hasPassphrase();
      bool hasHostname();
      bool hasIP();
      bool hasGateway();
      bool hasSubnet();
      
      char* toString();
      unsigned int maxLengthSSID() {return m_maxLengthSSID;}
      unsigned int maxLengthPassphrase() {return m_maxLengthPassphrase;}
      unsigned int maxLengthHostname() {return m_maxLengthHostname;}
      
    private:
      void format(const char* p_formatString, ...);
      
      const char* m_pSsid;
      const char* m_pPassphrase;
      const char* m_pHostname;
      uint32_t m_ip;
      uint32_t m_gateway;
      uint32_t m_subnet;
      
      static const unsigned int m_maxLengthSSID;
      static const unsigned int m_maxLengthPassphrase;
      static const unsigned int m_maxLengthHostname;
      
      char* m_pStringRepresentation;
  };
}
#endif