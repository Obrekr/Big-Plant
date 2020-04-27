#include "Logging.h"

Logging::Logging(Logging::Level level) {
  this->level = level;
  Serial.begin(115200);
}

void Logging::startup(std::string msg) {
  #if defined(LOGGING) && defined(LOGGING_STARTUP)
  Serial.println(msg.c_str());
  #endif
}

void Logging::log(Logging::Level lvl, Logging::Module mod, std::string msg) {
  #ifdef LOGGING
  bool isModuleActive = false;
  
  #ifdef LOGGING_GLOBAL 
  if(mod == Logging::GLOBAL) {isModuleActive = true;}
  #endif
  #ifdef LOGGING_BLE
  if(mod == Logging::BLE) {isModuleActive = true;}
  #endif
  #ifdef LOGGING_FLOWERCARE
  if(mod == Logging::FLOWERCARE) {isModuleActive = true;}
  #endif
  
  if(isModuleActive && lvl >= this->level) {
    Serial.println((this->getLevelString(lvl) + this->getModuleString(mod) + msg).c_str());
  }
  #endif
}

std::string Logging::getLevelString(Logging::Level lvl) {
  switch(lvl) {
    case Logging::DEBUG:
      return "DEBUG - ";
      break;
    case Logging::INFO:
      return "INFO - ";
      break;
    case Logging::WARNING:
      return "WARNING - ";
      break;
    case Logging::ERROR:
      return "ERROR - ";
      break;
  }
}

std::string Logging::getModuleString(Logging::Module mod) {
  switch(mod) {
    case Logging::GLOBAL:
      return "Global - ";
      break;
    case Logging::BLE:
      return "BLE - ";
      break;
    case Logging::FLOWERCARE:
      return "FlowerCare - ";
      break;
  }
}
