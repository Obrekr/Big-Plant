#ifndef Provisioning_Storage_h
#define Provisioning_Storage_h

#include "stddef.h"

namespace Provisioning{
  class Storage {
    public:
      virtual size_t putString(const char* p_key, const char* p_string) = 0;
      virtual size_t getString(const char* p_key, char* p_buffer, size_t bufferLength) = 0;
      
      virtual unsigned int putUInt(const char* p_key, unsigned int value) = 0;
      virtual unsigned int getUInt(const char* p_key, unsigned int defaultValue) = 0;
  };
}
#endif