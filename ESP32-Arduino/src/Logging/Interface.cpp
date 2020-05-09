#include "Interface.h"

namespace Logging {
  Interface::Interface(Logging::Facility facility, Logging::Level threshold, const char* p_name, LogManager* p_logging) 
                                    : m_facility(facility), m_threshold(threshold), m_pName(p_name), m_pLogging(p_logging) {

  }

  bool Interface::log(Logging::Level level, const char* p_format, ...) {
    // Should this message be logged according to own threshold?
    if(level > m_threshold) {
      return true;
    }
    
    va_list arguments;
    bool result;
    va_start(arguments, p_format);
    result = m_pLogging->log(level, m_facility, m_pName, Logging::format(p_format, arguments));
    va_end(arguments);

    return result;
  }

  bool Interface::emergency(const char* p_format, ...) {
    // Should this message be logged according to own threshold?
    if(Logging::EMERGENCY > m_threshold) {
      return true;
    }
    
    va_list arguments;
    bool result;
    va_start(arguments, p_format);
    result = m_pLogging->log(Logging::EMERGENCY, m_facility, m_pName, Logging::format(p_format, arguments));
    va_end(arguments);

    return result;
  }

  bool Interface::alert(const char* p_format, ...) {
    // Should this message be logged according to own threshold?
    if(Logging::ALERT > m_threshold) {
      return true;
    }
    
    va_list arguments;
    bool result;
    va_start(arguments, p_format);
    result = m_pLogging->log(Logging::ALERT, m_facility, m_pName, Logging::format(p_format, arguments));
    va_end(arguments);

    return result;
  }

  bool Interface::critical(const char* p_format, ...) {
    // Should this message be logged according to own threshold?
    if(Logging::CRITICAL > m_threshold) {
      return true;
    }
    
    va_list arguments;
    bool result;
    va_start(arguments, p_format);
    result = m_pLogging->log(Logging::CRITICAL, m_facility, m_pName, Logging::format(p_format, arguments));
    va_end(arguments);

    return result;
  }

  bool Interface::error(const char* p_format, ...) {
    // Should this message be logged according to own threshold?
    if(Logging::ERROR > m_threshold) {
      return true;
    }
    
    va_list arguments;
    bool result;
    va_start(arguments, p_format);
    result = m_pLogging->log(Logging::ERROR, m_facility, m_pName, Logging::format(p_format, arguments));
    va_end(arguments);

    return result;
  }

  bool Interface::warning(const char* p_format, ...) {
    // Should this message be logged according to own threshold?
    if(Logging::WARNING > m_threshold) {
      return true;
    }
    
    va_list arguments;
    bool result;
    va_start(arguments, p_format);
    result = m_pLogging->log(Logging::WARNING, m_facility, m_pName, Logging::format(p_format, arguments));
    va_end(arguments);

    return result;
  }

  bool Interface::notice(const char* p_format, ...) {
    // Should this message be logged according to own threshold?
    if(Logging::NOTICE > m_threshold) {
      return true;
    }
    
    va_list arguments;
    bool result;
    va_start(arguments, p_format);
    result = m_pLogging->log(Logging::NOTICE, m_facility, m_pName, Logging::format(p_format, arguments));
    va_end(arguments);

    return result;
  }

  bool Interface::info(const char* p_format, ...) {
    // Should this message be logged according to own threshold?
    if(Logging::INFO > m_threshold) {
      return true;
    }
    
    va_list arguments;
    bool result;
    va_start(arguments, p_format);
    result = m_pLogging->log(Logging::INFO, m_facility, m_pName, Logging::format(p_format, arguments));
    va_end(arguments);

    return result;
  }

  bool Interface::debug(const char* p_format, ...) {
    // Should this message be logged according to own threshold?
    if(Logging::DEBUG > m_threshold) {
      return true;
    }
    
    va_list arguments;
    bool result;
    va_start(arguments, p_format);
    result = m_pLogging->log(Logging::DEBUG, m_facility, m_pName, Logging::format(p_format, arguments));
    va_end(arguments);

    return result;
  }
}