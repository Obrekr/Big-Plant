#ifndef Logging_Output_h
#define Logging_Output_h

#include "Types.h"

namespace Logging {
  class Output {
    public:
      virtual bool send(Logging::Level level, Logging::Facility facility, const char* p_name, const char* p_message) = 0;
      virtual bool isReady() = 0;
  };
}
#endif