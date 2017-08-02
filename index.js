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
  this.eventsEnable = false;

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
  if (!this.eventsEnable) {
    this.eventsEnable = true;
    this.sampling();
  }
};

SoundSensor.prototype.disableEvents = function disableEvents() {
  this.eventsEnable = false;
};

SoundSensor.prototype.sampling = function sampling() {
  const self = this;
  function run() {
    if (!self.eventsEnable) {
      return;
    }
    const value = self.sound.getBasicValue();
    self.emit('medicion', value);
    setImmediate(run);
  }
  run();
};

SoundSensor.prototype.release = function release() {
  clearInterval(this.eventInterval);
  this.sound.release();
};

inherits(SoundSensor, EventEmitter);

module.exports = SoundSensor;
