#ifndef Utility_Utility_h
#define Utility_Utility_h

#include <stdio.h>
#include <stdarg.h>
#include <cstring>

namespace Utility {
  char* format(const char* p_formatString, ...);
}
#endif