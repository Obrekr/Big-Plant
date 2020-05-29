#include "Utility.h"

namespace Utility {
  char* format(const char* p_formatString, ...) {
    va_list arguments;
    va_start(arguments, p_formatString);
    
    char* result = new char();
    
    int finalLength = vsnprintf(NULL, 0, p_formatString, arguments);
    if(finalLength > 0) {
      delete[] result;
      result = new char[finalLength + 1];
      vsnprintf(result, finalLength + 1, p_formatString, arguments);
    }
    
    return result;
  }
}