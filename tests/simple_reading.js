const SoundSensor = require('../index');

const sound = new SoundSensor(2);

setInterval(() => {
  /* eslint-disable no-console */
  // console.log(`Value: ${sound.getValue().toFixed(3)}`);
  console.log(`BasicValue: ${sound.getBasicValue()}`);
}, 10);

process.on('SIGTERM', () => {
  process.exit();
});

process.on('SIGINT', () => {
  process.exit();
});
