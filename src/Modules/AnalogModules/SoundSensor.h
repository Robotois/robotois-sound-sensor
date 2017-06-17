#ifndef SOUNDSENSOR_H
#define SOUNDSENSOR_H

#include "../../Libraries/ADS1015/ADS1015.h"

class SoundSensor {
private:
  ADS1015 *analogModule;
  uint8_t inputPort;
  float scaleFactor;

public:
  SoundSensor (uint8_t _addr = 0x00);
  virtual ~SoundSensor ();

  void selectPort(uint8_t _port);
  float getValue();
  int16_t getBasicValue();
  // int16_t getScaledValue();

  void release();
};

#endif
