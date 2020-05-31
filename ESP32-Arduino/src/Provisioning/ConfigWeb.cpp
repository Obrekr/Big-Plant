#include "ConfigWeb.h"

namespace Provisioning {
  ConfigWeb::ConfigWeb(Storage* p_storage, ConfigWiFiAP* p_configWiFiAP, Webserver::Interface* p_webserver) : m_pStorage(p_storage), m_pConfigWiFiAP(p_configWiFiAP), m_pWebserver(p_webserver) {
    m_pWebServerConfig = new Webserver::Configuration();
    m_pKey = new unsigned char();
    m_pCACertificate = new unsigned char();
  }
  
  ConfigWeb::~ConfigWeb() {
    delete m_pWebServerConfig;
  }
  
  bool ConfigWeb::load() {
    unsigned int maxLength;
    size_t length;
    
    // Read key from NVS
    maxLength = m_pWebServerConfig->maxLengthKey();
    delete[] m_pKey;
    m_pKey = new unsigned char[maxLength + 1];
    length = m_pStorage->getBytes("webkey", m_pKey, maxLength);
    
    // Key may not be empty
    if(length == 0) {
      return false;
    }
    m_pKey[length] = '\0';
    m_pWebServerConfig->setKey(m_pKey);
    
    // Read certificate from NVS
    maxLength = m_pWebServerConfig->maxLengthCACertificate();
    delete[] m_pCACertificate;
    m_pCACertificate = new unsigned char[maxLength + 1];
    length = m_pStorage->getBytes("webcert", m_pCACertificate, maxLength);
    
    // Certificate may not be empty
    if(length == 0) {
      return false;
    }
    m_pCACertificate[length] = '\0';
    m_pWebServerConfig->setCACertificate(m_pCACertificate);
    
    return true;
  }
  
  bool ConfigWeb::save() {
    bool ret = true;
    
    if(m_pWebServerConfig->hasKey()) {
      ret &= m_pStorage->putBytes("webkey", m_pWebServerConfig->getKey(), strlen((char*) m_pWebServerConfig->getKey())) != 0;
    }
    if(m_pWebServerConfig->hasCACertificate()) {
      ret &= m_pStorage->putBytes("webcert", m_pWebServerConfig->getCACertificate(), strlen((char*) m_pWebServerConfig->getCACertificate())) != 0;
    }
    
    return ret;
  }
  
  bool ConfigWeb::generateDefault() {
    bool success;
    
    if(m_pConfigWiFiAP == NULL || !m_pConfigWiFiAP->get()->hasHostname()) {
      return false;
    }
    char* hostnameString = Utility::format("CN=%s,O=acme,C=DE", m_pConfigWiFiAP->get()->getHostname());
    success = m_pWebserver->generateSelfSignedCertificate(m_pWebServerConfig, 2048, hostnameString, "20200101000000", "20500101000000");
    delete[] hostnameString;
    
    return success && save();
  }
  
  Webserver::Configuration* ConfigWeb::get() {
    return m_pWebServerConfig;
  }
}