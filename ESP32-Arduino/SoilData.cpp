#include "SoilData.h"
SoilData::SoilData() {
  m_stringRepresentation = new char();
}

SoilData::~SoilData() {
  delete[] m_stringRepresentation;
}

char* SoilData::toString() {
  format("Temperature=%id°C Brightness=%ilux Moisture=%u%% Conductivity=%uuS/cm", m_temperature, m_brightness, m_moisture, m_conductivity);
  return m_stringRepresentation;
}

void SoilData::format(const char* p_formatString, ...) {
  va_list arguments;
  va_start(arguments, p_formatString);
  
  size_t finalLength = vsnprintf(NULL, 0, p_formatString, arguments);
  delete[] m_stringRepresentation;
  m_stringRepresentation = new char[finalLength + 1];
  vsnprintf(m_stringRepresentation, finalLength + 1, p_formatString, arguments);
}
