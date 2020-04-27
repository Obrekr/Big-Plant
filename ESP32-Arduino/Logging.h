#ifndef Logging_h
#define Logging_h

#include <string.h>
#include "Arduino.h"

#define LOGGING
#define LOGGING_STARTUP
#define LOGGING_GLOBAL
#define LOGGING_BLE
#define LOGGING_FLOWERCARE

class Logging {
  public:
    typedef enum {GLOBAL, BLE, FLOWERCARE} Module;
    typedef enum {DEBUG, INFO, WARNING, ERROR} Level;
    
    Logging(Logging::Level level);
    void startup(std::string msg);
    void log(Logging::Level lvl, Logging::Module mod, std::string msg);
    
    void debug(Logging::Module mod, std::string msg) {this->log(Logging::DEBUG, mod, msg);}
    void info(Logging::Module mod, std::string msg) {this->log(Logging::INFO, mod, msg);}
    void warning(Logging::Module mod, std::string msg) {this->log(Logging::WARNING, mod, msg);}
    void error(Logging::Module mod, std::string msg) {this->log(Logging::ERROR, mod, msg);}
 
  private:
    Logging::Level level;

    std::string getLevelString(Logging::Level lvl);
    std::string getModuleString(Logging::Module mod);
};
#endif
