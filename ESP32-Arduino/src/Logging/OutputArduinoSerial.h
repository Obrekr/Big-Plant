#ifndef Logging_OutputArduinoSerial_h
#define Logging_OutputArduinoSerial_h

#include "Types.h"
#include "Output.h"
#include "Arduino.h"

namespace Logging {
  class OutputArduinoSerial : public Output {
    public:
      OutputArduinoSerial(unsigned long baudrate);
      
      bool send(Logging::Level level, Logging::Facility facility, const char* p_name, const char* p_message);
      bool isReady();
  };
}
#endif