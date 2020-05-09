#ifndef Logging_Types_h
#define Logging_Types_h

#include <stdio.h>
#include <stdarg.h>
#include <cstring>

namespace Logging {
  typedef enum {NONE, EMERGENCY, ALERT, CRITICAL, ERROR, WARNING, NOTICE, INFO, DEBUG} Level;
  typedef enum {KERN, USER, MAIL, DAEMON, AUTH, SYSLOG, LPR, NEWS, UUCP, CRON, AUTHPRIV, FTP,
                LOCAL0, LOCAL1, LOCAL2, LOCAL3, LOCAL4, LOCAL5, LOCAL6, LOCAL7} Facility;
  
  const char* getLevelString(Logging::Level level);
  char* format(const char* p_formatString, ...);
  char* format(const char* p_formatString, va_list arguments);
}
#endif