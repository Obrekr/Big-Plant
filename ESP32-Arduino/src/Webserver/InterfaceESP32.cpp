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
    
    // Initialize entropy and rng
    mbedtls_entropy_context entropy;
    mbedtls_entropy_init(&entropy);
    
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ctr_drbg_init(&ctr_drbg);
      
    if(mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0) != 0) {
      mbedtls_entropy_free(&entropy);
      m_pLogger->error(m_pStringKeygenRNG);
      return false;
    }
    
    // Generate private key
    m_pLogger->info(m_pStringKeygenStart);
    mbedtls_pk_context key;
    mbedtls_pk_init(&key);
    if(mbedtls_pk_setup(&key, mbedtls_pk_info_from_type(MBEDTLS_PK_RSA)) != 0) {
      mbedtls_ctr_drbg_free(&ctr_drbg);
      mbedtls_entropy_free(&entropy);
      m_pLogger->error(m_pStringKeygenSetup);
      return false;
    }
    
    if(mbedtls_rsa_gen_key(mbedtls_pk_rsa( key ), mbedtls_ctr_drbg_random, &ctr_drbg, keyLength, 65537) != 0) {
      mbedtls_pk_free(&key);
      mbedtls_ctr_drbg_free(&ctr_drbg);
      mbedtls_entropy_free(&entropy);
      m_pLogger->error(m_pStringKeygenGenerate);
      return false;
    }
    
    // Generate X509 certificate with the private key
    m_pLogger->info(m_pStringCertgenStart);
    mbedtls_x509write_cert crt;
    mbedtls_x509write_crt_init(&crt);
    
    mbedtls_x509write_crt_set_version(&crt, MBEDTLS_X509_CRT_VERSION_3);
    mbedtls_x509write_crt_set_md_alg(&crt, MBEDTLS_MD_SHA256);
    mbedtls_x509write_crt_set_subject_key(&crt, &key);
    mbedtls_x509write_crt_set_issuer_key(&crt, &key);
    
    if(mbedtls_x509write_crt_set_subject_name(&crt, p_domainName) != 0 || mbedtls_x509write_crt_set_issuer_name(&crt, p_domainName) != 0) {
      mbedtls_x509write_crt_free(&crt);
      mbedtls_pk_free(&key);
      mbedtls_ctr_drbg_free(&ctr_drbg);
      mbedtls_entropy_free(&entropy);
      m_pLogger->error(m_pStringCertgenName, p_domainName);
      return false;
    }
    
    if(mbedtls_x509write_crt_set_validity(&crt, p_validFrom, p_validUntil) != 0 || mbedtls_x509write_crt_set_basic_constraints(&crt, 1, 0) != 0) {
      mbedtls_x509write_crt_free(&crt);
      mbedtls_pk_free(&key);
      mbedtls_ctr_drbg_free(&ctr_drbg);
      mbedtls_entropy_free(&entropy);
      m_pLogger->error(m_pStringCertgenValidFromUntil, p_validFrom, p_validUntil);
      return false;
    }
    
    m_pLogger->info(m_pStringCertgenSerialStart);
    mbedtls_mpi serial;
    mbedtls_mpi_init(&serial);
    if(mbedtls_mpi_read_string( &serial, 10, "1" ) != 0 || mbedtls_x509write_crt_set_serial( &crt, &serial ) != 0) {
      mbedtls_mpi_free(&serial);
      mbedtls_x509write_crt_free(&crt);
      mbedtls_pk_free(&key);
      mbedtls_ctr_drbg_free(&ctr_drbg);
      mbedtls_entropy_free(&entropy);
      m_pLogger->error(m_pStringCertgenSerial);
      return false;
    }
    
    // Save private key
    unsigned char* p_privateKey = new unsigned char[p_configuration->maxLengthPrivateKey()];
    if(mbedtls_pk_write_key_pem(&key, p_privateKey, p_configuration->maxLengthPrivateKey()) != 0) {
      mbedtls_mpi_free(&serial);
      mbedtls_x509write_crt_free(&crt);
      mbedtls_pk_free(&key);
      mbedtls_ctr_drbg_free(&ctr_drbg);
      mbedtls_entropy_free(&entropy);
      m_pLogger->error(m_pStringKeygenSave);
      return false;
    }
    p_configuration->setPrivateKey(p_privateKey);
    
    // Save X509 certificate
    unsigned char* p_x509Cert = new unsigned char[p_configuration->maxLengthCACertificate()];
    if(mbedtls_x509write_crt_pem(&crt, p_x509Cert, p_configuration->maxLengthCACertificate(), mbedtls_ctr_drbg_random, &ctr_drbg) != 0) {
      mbedtls_mpi_free(&serial);
      mbedtls_x509write_crt_free(&crt);
      mbedtls_pk_free(&key);
      mbedtls_ctr_drbg_free(&ctr_drbg);
      mbedtls_entropy_free(&entropy);
      m_pLogger->error(m_pStringCertgenSave);
      return false;
    }
    p_configuration->setCACertificate(p_x509Cert);
    
    // Clean up and return
    mbedtls_mpi_free(&serial);
    mbedtls_x509write_crt_free(&crt);
    mbedtls_pk_free(&key);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    m_pLogger->info(m_pStringSelfSignedSuccess);
    return true;
  }
}