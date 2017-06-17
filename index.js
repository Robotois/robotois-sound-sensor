const SSensor = require('bindings')('SoundSensor');
const EventEmitter = require('events').EventEmitter;
const inherits = require('util').inherits;

/**
 * Creates an instance of SoundSensor.
 * @param {int} port The port number where this component us connected.
 * @param {int} add The second argument.
 * @returns {SoundSensor} SoundSensor object.
 */
function SoundSensor(port, add = 0) {
  const self = this;
  EventEmitter.call(this);

  this.sound = new SSensor(port, add);

  process.on('SIGINT', () => {
    self.sound.release();
  });

  process.on('SIGTERM', () => {
    self.sound.release();
  });
}

SoundSensor.prototype.getValue = function getValue() {
  return this.sound.getValue();
};

SoundSensor.prototype.getBasicValue = function getBasicValue() {
  return this.sound.getBasicValue();
};

SoundSensor.prototype.enableEvents = function enableEvents() {
  if (!this.eventInterval) {
    let scaledValue;
    this.eventInterval = setInterval(() => {
      scaledValue = this.sound.getBasicValue();
      this.emit('medicion', scaledValue);
    }, 100); // Tomar mediciones cada 150ms
  }
};

// SoundSensor.prototype.when = function when(value, callback) {
//   this.enableEvents();
//   this.on('medicion', (soundValue) => {
//     console.log(`Posicion: ${soundValue}`);
//     if (value == soundValue) {
//       callback();
//     }
//   });
// };

SoundSensor.prototype.release = function release() {
  clearInterval(this.eventInterval);
  this.sound.release();
};

inherits(SoundSensor, EventEmitter);

module.exports = SoundSensor;
