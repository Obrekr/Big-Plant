#include "SoilData.h"

std::string SoilData::toString() {
  char stringConvert[16];
  std::string ret = "";

  itoa(m_temperature, stringConvert, 10);
  ret += "Temperature=" + std::string(stringConvert).insert(2, ".") + "°C ";
  itoa(m_brightness, stringConvert, 10);
  ret += "Brightness=" + std::string(stringConvert) + "lux ";
  itoa(m_moisture, stringConvert, 10);
  ret += "Moisture=" + std::string(stringConvert) + "% ";
  itoa(m_conductivity, stringConvert, 10);
  ret += "Conductivity=" + std::string(stringConvert) + "uS/cm";
  return ret;
}
