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
    // Values to be passed to a new task that will generate the private key and certificate
    CertificateSettings settings;
    settings.p_logger = m_pLogger;
    settings.p_configuration = p_configuration;
    settings.keyLength = keyLength;
    settings.p_domainName = p_domainName;
    settings.p_validFrom = p_validFrom;
    settings.p_validUntil = p_validUntil;
    settings.completed = false;
    settings.success = false;
    
    // Create the task and remove watchdog timer while generating private key and certificate
    disableCore0WDT();
    TaskHandle_t handle = NULL;
    xTaskCreate(taskSelfSignedCertificate, "genKeyCert", 8192, &settings, 1, &handle);
    if(handle == NULL) {
      // failed to create task
      return false;
    }

    
    // Wait for task to complete and enable watchdog again
    while(!settings.completed) {
      
    }
    enableCore0WDT();
    
    return settings.success;
  }
  
  void taskSelfSignedCertificate(void* parameter) {
    // Explicit conversion of parameter
    CertificateSettings* settings = (CertificateSettings*) parameter;
    
    settings->p_logger->info(InterfaceESP32::m_pStringSelfSignedStart);
    
    // Initialize entropy and rng
    mbedtls_entropy_context entropy;
    mbedtls_entropy_init(&entropy);
    
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ctr_drbg_init(&ctr_drbg);
      
    if(mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0) != 0) {
      mbedtls_entropy_free(&entropy);
      settings->p_logger->error(InterfaceESP32::m_pStringKeygenRNG);
      settings->success = false;
      settings->completed = true;
      vTaskDelete(NULL);
    }
    
    // Generate private key
    settings->p_logger->info(InterfaceESP32::m_pStringKeygenStart);
    mbedtls_pk_context key;
    mbedtls_pk_init(&key);
    if(mbedtls_pk_setup(&key, mbedtls_pk_info_from_type(MBEDTLS_PK_RSA)) != 0) {
      mbedtls_ctr_drbg_free(&ctr_drbg);
      mbedtls_entropy_free(&entropy);
      settings->p_logger->error(InterfaceESP32::m_pStringKeygenSetup);
      settings->success = false;
      settings->completed = true;
      vTaskDelete(NULL);
    }
    
    if(mbedtls_rsa_gen_key(mbedtls_pk_rsa(key), mbedtls_ctr_drbg_random, &ctr_drbg, settings->keyLength, 65537) != 0) {
      mbedtls_pk_free(&key);
      mbedtls_ctr_drbg_free(&ctr_drbg);
      mbedtls_entropy_free(&entropy);
      settings->p_logger->error(InterfaceESP32::m_pStringKeygenGenerate);
      settings->success = false;
      settings->completed = true;
      vTaskDelete(NULL);
    }
    
    // Generate X509 certificate with the private key
    settings->p_logger->info(InterfaceESP32::m_pStringCertgenStart);
    mbedtls_x509write_cert crt;
    mbedtls_x509write_crt_init(&crt);
    
    mbedtls_x509write_crt_set_version(&crt, MBEDTLS_X509_CRT_VERSION_3);
    mbedtls_x509write_crt_set_md_alg(&crt, MBEDTLS_MD_SHA256);
    mbedtls_x509write_crt_set_subject_key(&crt, &key);
    mbedtls_x509write_crt_set_issuer_key(&crt, &key);
    
    if(mbedtls_x509write_crt_set_subject_name(&crt, settings->p_domainName) != 0 || mbedtls_x509write_crt_set_issuer_name(&crt, settings->p_domainName) != 0) {
      mbedtls_x509write_crt_free(&crt);
      mbedtls_pk_free(&key);
      mbedtls_ctr_drbg_free(&ctr_drbg);
      mbedtls_entropy_free(&entropy);
      settings->p_logger->error(InterfaceESP32::m_pStringCertgenName, settings->p_domainName);
      settings->success = false;
      settings->completed = true;
      vTaskDelete(NULL);
    }
    
    if(mbedtls_x509write_crt_set_validity(&crt, settings->p_validFrom, settings->p_validUntil) != 0 || mbedtls_x509write_crt_set_basic_constraints(&crt, 1, 0) != 0) {
      mbedtls_x509write_crt_free(&crt);
      mbedtls_pk_free(&key);
      mbedtls_ctr_drbg_free(&ctr_drbg);
      mbedtls_entropy_free(&entropy);
      settings->p_logger->error(InterfaceESP32::m_pStringCertgenValidFromUntil, settings->p_validFrom, settings->p_validUntil);
      settings->success = false;
      settings->completed = true;
      vTaskDelete(NULL);
    }
    
    settings->p_logger->info(InterfaceESP32::m_pStringCertgenSerialStart);
    mbedtls_mpi serial;
    mbedtls_mpi_init(&serial);
    if(mbedtls_mpi_read_string( &serial, 10, "1" ) != 0 || mbedtls_x509write_crt_set_serial( &crt, &serial ) != 0) {
      mbedtls_mpi_free(&serial);
      mbedtls_x509write_crt_free(&crt);
      mbedtls_pk_free(&key);
      mbedtls_ctr_drbg_free(&ctr_drbg);
      mbedtls_entropy_free(&entropy);
      settings->p_logger->error(InterfaceESP32::m_pStringCertgenSerial);
      settings->success = false;
      settings->completed = true;
      vTaskDelete(NULL);
    }
    
    // Save private key
    unsigned char* p_privateKey = new unsigned char[settings->p_configuration->maxLengthPrivateKey()];
    if(mbedtls_pk_write_key_pem(&key, p_privateKey, settings->p_configuration->maxLengthPrivateKey()) != 0) {
      mbedtls_mpi_free(&serial);
      mbedtls_x509write_crt_free(&crt);
      mbedtls_pk_free(&key);
      mbedtls_ctr_drbg_free(&ctr_drbg);
      mbedtls_entropy_free(&entropy);
      settings->p_logger->error(InterfaceESP32::m_pStringKeygenSave);
      settings->success = false;
      settings->completed = true;
      vTaskDelete(NULL);
    }
    settings->p_configuration->setPrivateKey(p_privateKey);
    
    // Save X509 certificate
    unsigned char* p_x509Cert = new unsigned char[settings->p_configuration->maxLengthCACertificate()];
    if(mbedtls_x509write_crt_pem(&crt, p_x509Cert, settings->p_configuration->maxLengthCACertificate(), mbedtls_ctr_drbg_random, &ctr_drbg) != 0) {
      mbedtls_mpi_free(&serial);
      mbedtls_x509write_crt_free(&crt);
      mbedtls_pk_free(&key);
      mbedtls_ctr_drbg_free(&ctr_drbg);
      mbedtls_entropy_free(&entropy);
      settings->p_logger->error(InterfaceESP32::m_pStringCertgenSave);
      settings->success = false;
      settings->completed = true;
      vTaskDelete(NULL);
    }
    settings->p_configuration->setCACertificate(p_x509Cert);
    
    // Clean up and finish
    mbedtls_mpi_free(&serial);
    mbedtls_x509write_crt_free(&crt);
    mbedtls_pk_free(&key);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    settings->p_logger->info(InterfaceESP32::m_pStringSelfSignedSuccess);
    settings->success = true;
    settings->completed = true;
    vTaskDelete(NULL);
  }
}