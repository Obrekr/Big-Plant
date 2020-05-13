#include "ConfigWiFiAP.h"

namespace Provisioning {
  ConfigWiFiAP::ConfigWiFiAP(Storage* p_storage) : m_pStorage(p_storage) {
    m_pNetworkConfig = new Network::Configuration();
    m_pSSID = new char();
    m_pPassphrase  = new char();
    m_pHostname = new char();
    m_pNetworkConfig->setSSID(m_pSSID);
    m_pNetworkConfig->setPassphrase(m_pPassphrase);
    m_pNetworkConfig->setHostname(m_pHostname);
  }
  
  ConfigWiFiAP::~ConfigWiFiAP() {
    delete m_pNetworkConfig;
    delete[] m_pSSID;
    delete[] m_pPassphrase;
    delete[] m_pHostname;
  }
  
  bool ConfigWiFiAP::load() {
    char* buffer;
    uint8_t maxLength;
    size_t length;
    
    // Read SSID from NVS
    maxLength = m_pNetworkConfig->maxLengthSSID();
    buffer = new char[maxLength + 1];
    length = m_pStorage->getString("wifiapssid", buffer, maxLength);
    
    // SSID may not be empty, copy SSID to new cstring with correct length and free buffer
    if(length != 0) {
      delete[] m_pSSID;
      m_pSSID = new char[length + 1];
      strncpy(m_pSSID, buffer, length + 1);
      delete[] buffer;
      m_pNetworkConfig->setSSID(m_pSSID);
    } else {
      delete[] buffer;
      return false;
    }
    
    // Read passphrase from NVS
    maxLength = m_pNetworkConfig->maxLengthPassphrase();
    buffer = new char[maxLength + 1];
    length = m_pStorage->getString("wifiappass", buffer, maxLength);
    
    // Passphrase may not be empty, copy passphrase to new cstring with correct length and free buffer
    if(length != 0) {
      delete[] m_pPassphrase;
      m_pPassphrase = new char[length + 1];
      strncpy(m_pPassphrase, buffer, length + 1);
      delete[] buffer;
      m_pNetworkConfig->setPassphrase(m_pPassphrase);
    } else {
      delete[] buffer;
      return false;
    }
    
    // Read hostname from NVS
    maxLength = m_pNetworkConfig->maxLengthHostname();
    buffer = new char[maxLength + 1];
    length = m_pStorage->getString("wifiaphost", buffer, maxLength);
    
    // Copy hostname to new cstring with correct length and free buffer
    if(length != 0) {
      delete[] m_pHostname;
      m_pHostname = new char[length + 1];
      strncpy(m_pHostname, buffer, length + 1);
      delete[] buffer;
      m_pNetworkConfig->setHostname(m_pHostname);
    } else {
      delete[] buffer;
    }
    
    m_pNetworkConfig->setIP(m_pStorage->getUInt("wifiapip", 0));
    m_pNetworkConfig->setGateway(m_pStorage->getUInt("wifiapgate", 0));
    m_pNetworkConfig->setSubnet(m_pStorage->getUInt("wifiapsub", 0));
    
    return true;
  }
  
  bool ConfigWiFiAP::save() {
    bool ret = true;
    
    if(m_pNetworkConfig->hasSSID()) {
      ret &= m_pStorage->putString("wifiapssid", m_pNetworkConfig->getSSID()) != 0;
    }
    if(m_pNetworkConfig->hasPassphrase()) {
      ret &= m_pStorage->putString("wifiappass", m_pNetworkConfig->getPassphrase()) != 0;
    }
    if(m_pNetworkConfig->hasHostname()) {
      ret &= m_pStorage->putString("wifiaphost", m_pNetworkConfig->getHostname()) != 0;
    }
    
    ret &= m_pStorage->putUInt("wifiapip", m_pNetworkConfig->getIP()) != 0;
    ret &= m_pStorage->putUInt("wifiapgate", m_pNetworkConfig->getGateway()) != 0;
    ret &= m_pStorage->putUInt("wifiapsub", m_pNetworkConfig->getSubnet()) != 0;
    
    return ret;
  }
  
  bool ConfigWiFiAP::generateDefault() {
    m_pNetworkConfig->setSSID("Big Plant");
    m_pNetworkConfig->setPassphrase("bigplantwifi");
    m_pNetworkConfig->setHostname("bigplant.local");
    m_pNetworkConfig->setIP(((192) << 24) | ((168) << 16) | ((1) << 8) | 1);
    m_pNetworkConfig->setGateway(((192) << 24) | ((168) << 16) | ((1) << 8) | 1);
    m_pNetworkConfig->setSubnet(((255) << 24) | ((255) << 16) | ((255) << 8) | 0);
    return save();
  }
  
  Network::Configuration* ConfigWiFiAP::get() {
    return m_pNetworkConfig;
  }
}