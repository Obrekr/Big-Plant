#include "OutputArduinoSerial.h"

namespace Logging {
  OutputArduinoSerial::OutputArduinoSerial(unsigned long baudrate) {
    Serial.begin(baudrate);
  }
  
  bool OutputArduinoSerial::send(Logging::Level level, Logging::Facility facility, const char* p_name, const char* p_message) {
    const char outputFormatting[] = "%10lums %9s %s: %s";
    uint32_t currentTime = millis();
    
    char* message = Logging::format(outputFormatting, currentTime, Logging::getLevelString(level), p_name, p_message);
    Serial.println(message);
    delete[] message;
    
    return true;
  }
  
  bool OutputArduinoSerial::isReady() {
    return true;
  }
}