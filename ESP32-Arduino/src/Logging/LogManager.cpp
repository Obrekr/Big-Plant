#include "LogManager.h"

namespace Logging {
  bool LogManager::log(Logging::Level level, Logging::Facility facility, const char* p_name, const char* p_message) {
    // Send message to logging outputs and delete text from heap
    bool result = true;
    for(std::list<Logging::Output*>::iterator it = m_pLoggingOutputs.begin(); it != m_pLoggingOutputs.end(); ++it) {
      result &= (*it)->send(level, facility, p_name, p_message);
    }
    delete[] p_message;

    return result;
  }

  void LogManager::addOutput(Logging::Output* output) {
    m_pLoggingOutputs.insert(m_pLoggingOutputs.begin(), output);
  }
}