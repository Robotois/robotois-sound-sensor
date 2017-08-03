const Sound = require('../../index');
const Rotary = require('robotois-rotary-sensor');
const LCD = require('robotois-lcd-display');
const LED = require('robotois-led');

const lcd = new LCD();
const led = new LED(4);

const rotary = new Rotary(2);
rotary.enableEvents();
const sound = new Sound(1);
sound.enableEvents();

sound.on('medicion', (value) => {
  lcd.message(`Nivel: ${value}`);
});

rotary.on('medicion', (value) => {
  switch (true) {
    case (value > 5):
      led.blink();
      break;
    default:
      led.turnOff();
  }
});

process.on('SIGTERM', () => {
  process.exit();
});

process.on('SIGINT', () => {
  process.exit();
});
