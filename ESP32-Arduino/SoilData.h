#ifndef SoilData_h
#define SoilData_h

#include <string>
#include <stdlib.h>

class SoilData {
  public:
    SoilData() {};
    
    void setTemperature(int temperature) {m_temperature = temperature;}
    void setBrightness(int brightness) {m_brightness = brightness;}
    void setMoisture(unsigned int moisture) {m_moisture = moisture;}
    void setConductivity(unsigned int conductivity) {m_conductivity = conductivity;}
    int getTemperature() {return m_temperature;}
    int getBrightness() {return m_brightness;}
    unsigned int getMoisture() {return m_moisture;}
    unsigned int getConductivity() {return m_conductivity;}
    std::string toString();

  private:
    int m_temperature;
    int m_brightness;
    unsigned int m_moisture;
    unsigned int m_conductivity;
};
#endif
