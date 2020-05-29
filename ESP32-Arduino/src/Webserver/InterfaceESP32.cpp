#include "InterfaceESP32.h"

namespace Webserver {
  const char* InterfaceESP32::m_pStringSelfSignedStart = "Starting generation of self signed certificate";
  const char* InterfaceESP32::m_pStringKeygenStart = "Starting private key generation. This will take about 11 minutes";
  const char* InterfaceESP32::m_pStringKeygenRNG = "Creating RNG for key generation failed";
  const char* InterfaceESP32::m_pStringKeygenSetup = "Setup for key generation failed";
  const char* InterfaceESP32::m_pStringKeygenGenerate = "Generating private key failed";
  const char* InterfaceESP32::m_pStringKeygenSave = "Failed to save generated private key";
  const char* InterfaceESP32::m_pStringCertgenStart = "Starting X509 certificate generation";
  const char* InterfaceESP32::m_pStringCertgenName = "Setting domain name '%s' for certificate failed";
  const char* InterfaceESP32::m_pStringCertgenValidFromUntil = "Setting valid from '%s' until '%s' for certificate failed";
  const char* InterfaceESP32::m_pStringCertgenSerialStart = "Started calculating serial number for certificate";
  const char* InterfaceESP32::m_pStringCertgenSerial = "Generating serial number for certificate failed";
  const char* InterfaceESP32::m_pStringCertgenSave = "Failed to save generated certificate";
  const char* InterfaceESP32::m_pStringSelfSignedSave = "Saving self signed certificate";
  const char* InterfaceESP32::m_pStringSelfSignedSuccess = "Successfully created self signed certificate";
  
  InterfaceESP32::InterfaceESP32(Logging::Interface* p_logger) : m_pLogger(p_logger) {
    
  }
  
  bool InterfaceESP32::start() {
    return true;
  }
  
  bool InterfaceESP32::stop() {
    return true;
  }
  
  bool InterfaceESP32::generateSelfSignedCertificate(Configuration* p_configuration, unsigned int keyLength, const char* p_domainName,
                                                     const char* p_validFrom, const char* p_validUntil) {
    m_pLogger->info(m_pStringSelfSignedStart);
    bool ret;
    mbedtls_pk_context* key = NULL;
    mbedtls_x509write_cert* crt;
    mbedtls_mpi* serial;
    unsigned char* p_privateKey = NULL;
    unsigned char* p_x509Cert = NULL;
    
    // Initialize entropy and rng
    mbedtls_entropy_context* entropy = new mbedtls_entropy_context;
    mbedtls_entropy_init(entropy);
    
    mbedtls_ctr_drbg_context* ctr_drbg = new mbedtls_ctr_drbg_context;
    mbedtls_ctr_drbg_init(ctr_drbg);
      
    if(mbedtls_ctr_drbg_seed(ctr_drbg, mbedtls_entropy_func, entropy, NULL, 0) != 0) {
      m_pLogger->error(m_pStringKeygenRNG);
      ret = false;
      goto cleanupEntropy;
    }
    
    // Generate private key
    m_pLogger->info(m_pStringKeygenStart);
    key = new mbedtls_pk_context;
    mbedtls_pk_init(key);
    if(mbedtls_pk_setup(key, mbedtls_pk_info_from_type(MBEDTLS_PK_RSA)) != 0) {
      m_pLogger->error(m_pStringKeygenSetup);
      ret = false;
      goto cleanupDrbg;
    }
    
    if(mbedtls_rsa_gen_key(mbedtls_pk_rsa(*key), mbedtls_ctr_drbg_random, ctr_drbg, keyLength, 65537) != 0) {
      m_pLogger->error(m_pStringKeygenGenerate);
      ret = false;
      goto cleanupPk;
    }
    
    // Generate X509 certificate with the private key
    m_pLogger->info(m_pStringCertgenStart);
    crt = new mbedtls_x509write_cert;
    mbedtls_x509write_crt_init(crt);
    
    mbedtls_x509write_crt_set_version(crt, MBEDTLS_X509_CRT_VERSION_3);
    mbedtls_x509write_crt_set_md_alg(crt, MBEDTLS_MD_SHA256);
    mbedtls_x509write_crt_set_subject_key(crt, key);
    mbedtls_x509write_crt_set_issuer_key(crt, key);
    
    if(mbedtls_x509write_crt_set_subject_name(crt, p_domainName) != 0 || mbedtls_x509write_crt_set_issuer_name(crt, p_domainName) != 0) {
      m_pLogger->error(m_pStringCertgenName, p_domainName);
      ret = false;
      goto cleanupX509write;
    }
    
    if(mbedtls_x509write_crt_set_validity(crt, p_validFrom, p_validUntil) != 0 || mbedtls_x509write_crt_set_basic_constraints(crt, 1, 0) != 0) {
      m_pLogger->error(m_pStringCertgenValidFromUntil, p_validFrom, p_validUntil);
      ret = false;
      goto cleanupX509write;
    }
    
    m_pLogger->info(m_pStringCertgenSerialStart);
    serial = new mbedtls_mpi;
    mbedtls_mpi_init(serial);
    if(mbedtls_mpi_read_string( serial, 10, "1" ) != 0 || mbedtls_x509write_crt_set_serial( crt, serial ) != 0) {
      m_pLogger->error(m_pStringCertgenSerial);
      ret = false;
      goto cleanupMpi;
    }
    mbedtls_mpi_free(serial);
    delete serial;
    
    // Save private key
    m_pLogger->info(m_pStringSelfSignedSave);
    p_privateKey = new unsigned char[p_configuration->maxLengthPrivateKey()];
    if(mbedtls_pk_write_key_pem(key, p_privateKey, p_configuration->maxLengthPrivateKey()) != 0) {
      m_pLogger->error(m_pStringKeygenSave);
      delete p_privateKey;
      ret = false;
      goto cleanupX509write;
    }
    p_configuration->setPrivateKey(p_privateKey);
    
    // Save X509 certificate
    p_x509Cert = new unsigned char[p_configuration->maxLengthCACertificate()];
    if(mbedtls_x509write_crt_pem(crt, p_x509Cert, p_configuration->maxLengthCACertificate(), mbedtls_ctr_drbg_random, ctr_drbg) != 0) {
      m_pLogger->error(m_pStringCertgenSave);
      delete p_x509Cert;
      ret = false;
      goto cleanupX509write;
    }
    p_configuration->setCACertificate(p_x509Cert);
    
    m_pLogger->info(m_pStringSelfSignedSuccess);
    ret = true;
    goto cleanupX509write;
    
    // Clean up and return
  cleanupMpi:
    mbedtls_mpi_free(serial);
    delete serial;
  cleanupX509write:
    mbedtls_x509write_crt_free(crt);
    delete crt;
  cleanupPk:
    mbedtls_pk_free(key);
  cleanupDrbg:
    mbedtls_ctr_drbg_free(ctr_drbg);
    delete key;
  cleanupEntropy:
    mbedtls_entropy_free(entropy);
    delete ctr_drbg;
    delete entropy;
    return ret;
  }
}