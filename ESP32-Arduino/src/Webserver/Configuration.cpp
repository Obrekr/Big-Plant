#include "Configuration.h"

namespace Webserver {
  const unsigned int Configuration::m_maxLengthCACertificate = 4097;
  const unsigned int Configuration::m_maxLengthKey = 4097;
  
  Configuration::Configuration() : m_pCACertificate(NULL), m_pPrivateKey(NULL) {
    
  }
  
  Configuration::~Configuration() {
    
  }
}