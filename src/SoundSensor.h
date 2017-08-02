#ifndef SOUNDSENSOR_H
#define SOUNDSENSOR_H

#include "./libraries/robotois-ADS1015/ADS1015.h"

class SoundSensor {
private:
  ADS1015 *analogModule;
  uint8_t adsPort;
  float scaleFactor;

public:
  SoundSensor (uint8_t _addr = 0x00);
  virtual ~SoundSensor ();

  void selectPort(uint8_t _port);
  float getValue();
  uint8_t getBasicValue();

  void release();
};

#endif
