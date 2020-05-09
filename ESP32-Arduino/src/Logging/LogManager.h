#ifndef Logging_LogManager_h
#define Logging_LogManager_h

#include <list>
#include "Types.h"
#include "Output.h"

namespace Logging {
  class LogManager {
    public:
      bool log(Logging::Level level, Logging::Facility facility, const char* p_name, const char* p_message);
      void addOutput(Logging::Output* output);

    private:
      std::list<Logging::Output*> m_pLoggingOutputs;
  };
}
#endif
