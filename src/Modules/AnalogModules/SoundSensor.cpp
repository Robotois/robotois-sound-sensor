#include <iostream>
#include <cmath>
#include "SoundSensor.h"

SoundSensor::SoundSensor(uint8_t _addr){
  analogModule = new ADS1015(_addr);
  scaleFactor = 100.0f / 2.5f; // Voltage range => (-2) - 2
}

SoundSensor::~SoundSensor(){
}

void SoundSensor::selectPort(uint8_t _port){
  inputPort = _port;
  switch(inputPort){
      case 0x01:
          analogModule->selectInput(ADS1015_SENSOR1,ADS1015_6144_GAIN);
          break;
      case 0x02:
          analogModule->selectInput(ADS1015_SENSOR2,ADS1015_6144_GAIN);
          break;
      case 0x03:
          analogModule->selectInput(ADS1015_SENSOR3,ADS1015_6144_GAIN);
          break;
      case 0x04:
          analogModule->selectInput(ADS1015_SENSOR4,ADS1015_6144_GAIN);
          break;
      default:
          printf("Error selecting the Analog Port...\n");
          return;
  }
}

/**
 * Returns the analog value in volts, for this particular case the value is in the
 * range of (-2.5) - 2.5 Volts.
 * @return
 */
float SoundSensor::getValue(){
  selectPort(inputPort);
  return 2.5f - analogModule->readInput();
}

int16_t SoundSensor::getBasicValue(){
  selectPort(inputPort);
  // int16_t inputShifted = getValue();
  // float value = getValue();
  // printf("Value: %f - ", value);
  // int16_t sound = std::round(getValue() * scaleFactor);
  return (int16_t) (std::round(getValue() * scaleFactor));
}

void SoundSensor::release(){
  delete analogModule;
  printf("[SoundSensor] => Released\n");
}
