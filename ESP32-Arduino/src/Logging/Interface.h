#ifndef Logging_Interface_h
#define Logging_Interface_h

#include "LogManager.h"
#include <stdarg.h>

namespace Logging {
  class Interface {
    public:
      Interface(Logging::Facility facility, Logging::Level threshold, const char* p_name, LogManager* p_logging);

      bool log(Logging::Level level, const char* p_format, ...);
      bool emergency(const char* p_format, ...);
      bool alert(const char* p_format, ...);
      bool critical(const char* p_format, ...);
      bool error(const char* p_format, ...);
      bool warning(const char* p_format, ...);
      bool notice(const char* p_format, ...);
      bool info(const char* p_format, ...);
      bool debug(const char* p_format, ...);
    
      Logging::Facility getFacility() {return m_facility;}
      Logging::Level getThreshold() {return m_threshold;}
      const char* getName() {return m_pName;}

    private:
      Logging::Facility m_facility;
      Logging::Level m_threshold;
      const char* m_pName;
      LogManager* m_pLogging;
  };
}
#endif
