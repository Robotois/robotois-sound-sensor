const SoundSensor = require('../index');

const sound = new SoundSensor(1);

sound.enableEvents();

sound.on('medicion', (value) => {
  /* eslint-disable no-console */
  console.log(`Sonido: ${value}`);
});

process.on('SIGTERM', () => {
  process.exit();
});

process.on('SIGINT', () => {
  process.exit();
});
