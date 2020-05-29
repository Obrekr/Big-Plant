#ifndef Webserver_Configuration_h
#define Webserver_Configuration_h

#include <cstring>

namespace Webserver {
  class Configuration {
    public:
      Configuration();
      ~Configuration();
      
      void setCACertificate(unsigned char* p_certificate) {m_pCACertificate = p_certificate;}
      void setPrivateKey(unsigned char* p_privateKey) {m_pPrivateKey = p_privateKey;}
      
      bool hasCACertificate() {return m_pCACertificate != NULL && strlen((char*) m_pCACertificate) != 0;}
      bool hasPrivateKey() {return m_pPrivateKey != NULL && strlen((char*) m_pPrivateKey) != 0;}
      
      unsigned char* getCACertificate() {return m_pCACertificate;}
      unsigned char* getPrivateKey() {return m_pPrivateKey;}
      
      unsigned int maxLengthCACertificate() {return m_maxLengthCACertificate;}
      unsigned int maxLengthPrivateKey() {return m_maxLengthPrivateKey;}
      
    private:
      unsigned char* m_pCACertificate;
      unsigned char* m_pPrivateKey;
      
      static const unsigned int m_maxLengthCACertificate;
      static const unsigned int m_maxLengthPrivateKey;
  };
}
#endif