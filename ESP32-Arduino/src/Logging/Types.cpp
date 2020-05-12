#include "Types.h"

namespace Logging {
  const char* getLevelString(Logging::Level level) {
    switch(level) {
      case Logging::NONE:
        return "";
        break;
      case Logging::EMERGENCY:
        return "EMERGENCY";
        break;
      case Logging::ALERT:
        return "ALERT";
        break;
      case Logging::CRITICAL:
        return "CRITICAL";
        break;
      case Logging::ERROR:
        return "ERROR";
        break;
      case Logging::WARNING:
        return "WARNING";
        break;
      case Logging::NOTICE:
        return "NOTICE";
        break;
      case Logging::INFO:
        return "INFO";
        break;
      case Logging::DEBUG:
        return "DEBUG";
        break;
      default:
        return "";
        break;
    }
  }
  
  char* format(const char* p_formatString, ...) {
    va_list arguments;
    va_start(arguments, p_formatString);
    
    char* string = format(p_formatString, arguments);
    
    va_end(arguments);
    return string;
  }
  
  char* format(const char* p_formatString, va_list arguments) {
    size_t formatLength = strlen(p_formatString);
    char* string = new char[formatLength + 1];
    size_t finalLength = vsnprintf(string, formatLength + 1, p_formatString, arguments);
    
    if(finalLength > formatLength) {
      delete[] string;
      string = new char[finalLength + 1];
      vsnprintf(string, finalLength + 1, p_formatString, arguments);
    }
    
    return string;
  }
}