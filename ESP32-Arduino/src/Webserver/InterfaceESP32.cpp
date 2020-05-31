#include "InterfaceESP32.h"

namespace Webserver {
  esp_err_t root_get_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>Hello Secure World!</h1>", -1); // -1 = use strlen()
    return ESP_OK;
  }
  const httpd_uri_t uriRoot = {.uri = "/", .method = HTTP_GET, .handler = root_get_handler};
  
  const char* InterfaceESP32::m_pStringWebserverStart = "Starting webserver";
  const char* InterfaceESP32::m_pStringWebserverNoCert = "Configuration has no x509 certificate";
  const char* InterfaceESP32::m_pStringWebserverNoKey = "Configuration has no key";
  const char* InterfaceESP32::m_pStringWebserverStartFail = "Failed to start webserver";
  const char* InterfaceESP32::m_pStringWebserverStartSuccess = "Successfully started webserver";
  const char* InterfaceESP32::m_pStringWebserverStop = "Webserver stopped";
  const char* InterfaceESP32::m_pStringSelfSignedStart = "Starting generation of self signed certificate";
  const char* InterfaceESP32::m_pStringSelfSignedKeygenFail = "Failed to generate public key";
  const char* InterfaceESP32::m_pStringSelfSignedCertgenFail = "Failed to generate x509 certificate";
  const char* InterfaceESP32::m_pStringSelfSignedSuccess = "Successfully created self signed certificate";
  const char* InterfaceESP32::m_pStringKeygenStart = "Starting public key generation, this will take some time";
  const char* InterfaceESP32::m_pStringKeygenRNG = "Creating RNG for key generation failed";
  const char* InterfaceESP32::m_pStringKeygenSetup = "Setup for key generation failed";
  const char* InterfaceESP32::m_pStringKeygenGenerate = "Generating public key failed";
  const char* InterfaceESP32::m_pStringKeygenSave = "Saving public key";
  const char* InterfaceESP32::m_pStringKeygenOutOfMemory = "Out of memory for saveing public key";
  const char* InterfaceESP32::m_pStringKeygenSaveFail = "Failed to save generated public key";
  const char* InterfaceESP32::m_pStringCertgenStart = "Starting x509 certificate generation";
  const char* InterfaceESP32::m_pStringCertgenRNG = "Creating RNG for key generation failed";
  const char* InterfaceESP32::m_pStringCertgenPK = "Failed to load the public key";
  const char* InterfaceESP32::m_pStringCertgenName = "Setting domain name for certificate to '%s'";
  const char* InterfaceESP32::m_pStringCertgenNameFail = "Setting domain name '%s' for certificate failed";
  const char* InterfaceESP32::m_pStringCertgenValidFromUntil = "Setting valid from to '%s' and until to '%s' for certificate";
  const char* InterfaceESP32::m_pStringCertgenValidFromUntilFail = "Setting valid from '%s' until '%s' for certificate failed";
  const char* InterfaceESP32::m_pStringCertgenSerialStart = "Started calculating serial number for certificate";
  const char* InterfaceESP32::m_pStringCertgenSerial = "Generating serial number for certificate failed";
  const char* InterfaceESP32::m_pStringCertgenSave = "Saving x509 certificate";
  const char* InterfaceESP32::m_pStringCertgenOutOfMemory = "Out of memory for saving x509 certificate";
  const char* InterfaceESP32::m_pStringCertgenSaveFail = "Failed to save generated certificate";
  
  InterfaceESP32::InterfaceESP32(Logging::Interface* p_logger) : m_pLogger(p_logger) {
    
  }
  
  bool InterfaceESP32::start(Configuration* p_configuration) {
    m_pLogger->info(m_pStringWebserverStart);
    
    // Configure https webserver
    httpd_ssl_config_t conf = HTTPD_SSL_CONFIG_DEFAULT();
    if(!p_configuration->hasCACertificate()) {
      m_pLogger->error(m_pStringWebserverNoCert);
      return false;
    }
    conf.cacert_pem = p_configuration->getCACertificate();
    conf.cacert_len = strlen((char*) p_configuration->getCACertificate() + 1);
    
    if(!p_configuration->hasKey()) {
      m_pLogger->error(m_pStringWebserverNoKey);
      return false;
    }
    conf.prvtkey_pem = p_configuration->getKey();
    conf.prvtkey_len = strlen((char*) p_configuration->getKey() + 1);
    
    // Start the webserver
    if(httpd_ssl_start(&m_webserver, &conf) != ESP_OK) {
      m_pLogger->error(m_pStringWebserverStartFail);
      return false;
    }
    
    // Register URI handlers
    httpd_register_uri_handler(m_webserver, &uriRoot);
    
    m_pLogger->info(m_pStringWebserverStartSuccess);
    return true;
  }
  
  bool InterfaceESP32::stop() {
    httpd_ssl_stop(m_webserver);
    m_pLogger->info(m_pStringWebserverStop);
    return true;
  }
  
  bool InterfaceESP32::generateSelfSignedCertificate(Configuration* p_configuration, unsigned int keyLength, const char* p_domainName,
      const char* p_validFrom, const char* p_validUntil) {
      m_pLogger->info(m_pStringSelfSignedStart);
      
      if(!generateKey(p_configuration, keyLength)) {
        m_pLogger->error(m_pStringSelfSignedKeygenFail);
        return false;
      }
      
      if(!generateCACertificate(p_configuration, p_domainName, p_validFrom, p_validUntil)) {
        m_pLogger->error(m_pStringSelfSignedCertgenFail);
        return false;
      }
      
      m_pLogger->info(m_pStringSelfSignedSuccess);
      return true;
    }
    
  bool InterfaceESP32::generateKey(Configuration* p_configuration, unsigned int keyLength) {
    m_pLogger->info(m_pStringKeygenStart);
    
    bool ret;
    mbedtls_pk_context* key = NULL;
    unsigned char* p_publicKey = NULL;
    
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
    
    // Generate public key
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
    
    // Save public key
    m_pLogger->info(m_pStringKeygenSave);
    p_publicKey = new unsigned char[p_configuration->maxLengthKey()];
    if(p_publicKey == NULL) {
      m_pLogger->error(m_pStringKeygenOutOfMemory);
      ret = false;
      goto cleanupPk;
    }
    if(mbedtls_pk_write_key_pem(key, p_publicKey, p_configuration->maxLengthKey()) != 0) {
      m_pLogger->error(m_pStringKeygenSaveFail);
      delete[] p_publicKey;
      ret = false;
      goto cleanupPk;
    }
    p_configuration->setKey(p_publicKey);
    ret = true;
    
    // Clean up and return
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
  
  bool InterfaceESP32::generateCACertificate(Configuration* p_configuration, const char* p_domainName, const char* p_validFrom, const char* p_validUntil) {
    m_pLogger->info(m_pStringCertgenStart);
    
    bool ret;
    mbedtls_pk_context* key = NULL;
    mbedtls_x509write_cert* crt;
    mbedtls_mpi* serial;
    unsigned char* p_x509CertDER = NULL;
    unsigned char* p_x509CertPEM = NULL;
    int p_x509CertDERLength = 0;
    size_t p_x509CertPEMLength = 0;
    
    // Initialize entropy and rng
    mbedtls_entropy_context* entropy = new mbedtls_entropy_context;
    mbedtls_entropy_init(entropy);
    
    mbedtls_ctr_drbg_context* ctr_drbg = new mbedtls_ctr_drbg_context;
    mbedtls_ctr_drbg_init(ctr_drbg);
      
    if(mbedtls_ctr_drbg_seed(ctr_drbg, mbedtls_entropy_func, entropy, NULL, 0) != 0) {
      m_pLogger->error(m_pStringCertgenRNG);
      ret = false;
      goto cleanupEntropy;
    }
    
    // Load public key
    key = new mbedtls_pk_context;
    mbedtls_pk_init(key);
    if(!p_configuration->hasKey() || mbedtls_pk_parse_key(key, p_configuration->getKey(), strlen((char*) p_configuration->getKey()) + 1, NULL, 0) != 0) {
      m_pLogger->error(m_pStringCertgenPK);
      ret = false;
      goto cleanupDrbg;
    }
    
    // Generate X509 certificate with the public key
    crt = new mbedtls_x509write_cert;
    mbedtls_x509write_crt_init(crt);
    
    mbedtls_x509write_crt_set_version(crt, MBEDTLS_X509_CRT_VERSION_3);
    mbedtls_x509write_crt_set_md_alg(crt, MBEDTLS_MD_SHA256);
    mbedtls_x509write_crt_set_subject_key(crt, key);
    mbedtls_x509write_crt_set_issuer_key(crt, key);
    
    m_pLogger->info(m_pStringCertgenName, p_domainName);
    if(mbedtls_x509write_crt_set_subject_name(crt, p_domainName) != 0 || mbedtls_x509write_crt_set_issuer_name(crt, p_domainName) != 0) {
      m_pLogger->error(m_pStringCertgenNameFail, p_domainName);
      ret = false;
      goto cleanupX509write;
    }
    
    m_pLogger->info(m_pStringCertgenValidFromUntil, p_validFrom, p_validUntil);
    if(mbedtls_x509write_crt_set_validity(crt, p_validFrom, p_validUntil) != 0 || mbedtls_x509write_crt_set_basic_constraints(crt, 1, 0) != 0) {
      m_pLogger->error(m_pStringCertgenValidFromUntilFail, p_validFrom, p_validUntil);
      ret = false;
      goto cleanupX509write;
    }
    
    m_pLogger->info(m_pStringCertgenSerialStart);
    serial = new mbedtls_mpi;
    mbedtls_mpi_init(serial);
    if(mbedtls_mpi_read_string(serial, 10, "1") != 0 || mbedtls_x509write_crt_set_serial(crt, serial) != 0) {
      m_pLogger->error(m_pStringCertgenSerial);
      ret = false;
      goto cleanupMpi;
    }
    
    // Save X509 certificate as PEM
    m_pLogger->info(m_pStringCertgenSave);
    p_x509CertDER = new unsigned char[p_configuration->maxLengthCACertificate()];
    p_x509CertPEM = new unsigned char[p_configuration->maxLengthCACertificate()];
    if(p_x509CertDER == NULL || p_x509CertPEM == NULL) {
      m_pLogger->error(m_pStringCertgenOutOfMemory);
      ret = false;
      goto cleanupMpi;
    }
   /* Save as DER certificate first. mbedtls_x509write_crt_der(...) will use 2048 bytes on the stack.
    * Do not use mbedtls_x509write_crt_pem(...) as it will us 4096 bytes on the stack and call
    * mbedtls_x509write_crt_der(...) anyway. This will cause a stack overflow.
    * Instead convert DER certificate to PEM directly.
    */
    if((p_x509CertDERLength = mbedtls_x509write_crt_der(crt, p_x509CertDER, p_configuration->maxLengthCACertificate(), mbedtls_ctr_drbg_random, ctr_drbg)) < 0
       || mbedtls_pem_write_buffer("-----BEGIN CERTIFICATE-----\n", "-----END CERTIFICATE-----\n",
                                   p_x509CertDER + p_configuration->maxLengthCACertificate() - p_x509CertDERLength, p_x509CertDERLength,
                                   p_x509CertPEM, p_configuration->maxLengthCACertificate(), &p_x509CertPEMLength) != 0) {
      m_pLogger->error(m_pStringCertgenSaveFail);
      delete[] p_x509CertDER;
      delete[] p_x509CertPEM;
      ret = false;
      goto cleanupMpi;
    }
    delete[] p_x509CertDER;
    p_configuration->setCACertificate(p_x509CertPEM);
    ret = true;
    
    // Clean up and return
  cleanupMpi:
    mbedtls_mpi_free(serial);
    delete serial;
  cleanupX509write:
    mbedtls_x509write_crt_free(crt);
    delete crt;
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