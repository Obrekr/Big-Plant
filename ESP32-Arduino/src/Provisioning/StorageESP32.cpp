#include "StorageESP32.h"

namespace Provisioning {
  const char* StorageESP32::m_pStringNVSFail = "Failed to open prefrences on NVS";
  
  StorageESP32::StorageESP32(Logging::Interface* p_logger) : m_pLogger(p_logger) {
    if(!preferences.begin("bigplant", false)) {
      m_pLogger->error(m_pStringNVSFail);
      return;
    }
  }
  StorageESP32::~StorageESP32() {
    preferences.end();
  }
  
  size_t StorageESP32::putBytes(const char* p_key, const void* p_value, size_t length) {
    return preferences.putBytes(p_key, p_value, length);
  }
  
  size_t StorageESP32::getBytesLength(const char* p_key) {
    return preferences.getBytesLength(p_key);
  }
  
  size_t StorageESP32::getBytes(const char* p_key, void * p_buffer, size_t bufferLength) {
    return preferences.getBytes(p_key, p_buffer, bufferLength);
  }
  
  size_t StorageESP32::putString(const char* p_key, const char* p_string) {
    return preferences.putString(p_key, p_string);
  }
  
  size_t StorageESP32::getString(const char* p_key, char* p_buffer, size_t bufferLength) {
    return preferences.getString(p_key, p_buffer, bufferLength);
  }
  
  unsigned int StorageESP32::getUInt(const char* p_key, unsigned int defaultValue) {
    return preferences.getUInt(p_key, defaultValue);
  }
  
  size_t StorageESP32::putUInt(const char* p_key, unsigned int value) {
    return preferences.putUInt(p_key, value);
  }
}
