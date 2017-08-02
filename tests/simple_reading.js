const SoundSensor = require('../index');

const sound = new SoundSensor(1);

setInterval(() => {
  /* eslint-disable no-console */
  // console.log(`Value: ${sound.getValue().toFixed(3)}`);
  console.log(`BasicValue: ${sound.getBasicValue()}`);
}, 50);

process.on('SIGTERM', () => {
  process.exit();
});

process.on('SIGINT', () => {
  process.exit();
});
