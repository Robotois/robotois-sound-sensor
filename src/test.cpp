#include <cstdio>
#include <bcm2835.h>

#include "./Libraries/ADS1015/ADS1015.h"
#include "./Modules/AnalogModules/SoundSensor.h"
#include "./Libraries/Timer/AccurateTiming.h"

void i2c_init();
void i2c_end();

int main(int argc, char const *argv[]) {
  i2c_init();
  SoundSensor sound;
  while (true) {
    sound.selectPort(2);
    printf("getValue: %f, getBasicValue: %d\n",
      sound.getValue(),
      sound.getBasicValue()
    );
    mDelay(10);
  }
  i2c_end();
  return 0;
}

void i2c_init() {
  uint16_t clkDiv = BCM2835_I2C_CLOCK_DIVIDER_626;

  if(!bcm2835_init()){
    printf("BCM2835 Error!!!\n");
  }

  bcm2835_i2c_begin();

  bcm2835_i2c_setClockDivider(clkDiv);
}

void i2c_end() {
  bcm2835_i2c_end();
  bcm2835_close();
}
