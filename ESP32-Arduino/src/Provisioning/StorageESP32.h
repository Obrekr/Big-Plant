#ifndef Provisioning_StorageESP32_h
#define Provisioning_StorageESP32_h

#include <Preferences.h>
#include "Storage.h"
#include "../Logging/Interface.h"

namespace Provisioning{
  class StorageESP32 : public Storage {
    public:
      StorageESP32(Logging::Interface* p_logger);
      ~StorageESP32();
      
      size_t putBytes(const char* p_key, const void* p_value, size_t length);
      size_t getBytesLength(const char* p_key);
      size_t getBytes(const char* p_key, void * p_buffer, size_t bufferLength);
      
      size_t putString(const char* p_key, const char* p_string);
      size_t getString(const char* p_key, char* p_buffer, size_t bufferLength);
      
      size_t putUInt(const char* p_key, unsigned int value);
      unsigned int getUInt(const char* p_key, unsigned int defaultValue);
      
      bool remove(const char* p_key);
    private:
      Preferences preferences;
      Logging::Interface* m_pLogger;
      static const char* m_pStringNVSFail;
  };
}
#endif