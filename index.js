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
  this.prevValue = 0;

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

SoundSensor.prototype.enableEvents = function enableEvents(mqttConfig) {
  if (mqttConfig) {
    this.mqttClient = mqttConfig.mqttClient;
    this.myTopic = `sensors/sound${mqttConfig.instance}`;
    this.mqttClient.publish('registerTopic', this.myTopic);
  }
  if (!this.eventsEnable) {
    this.eventsEnable = true;
    this.sampling();
  }
};

SoundSensor.prototype.disableEvents = function disableEvents() {
  this.eventsEnable = false;
};

SoundSensor.prototype.publishNow = function publishNow() {
  this.mqttClient.publish(this.myTopic, this.sound.getBasicValue().toString());
};

SoundSensor.prototype.sampling = function sampling() {
  const self = this;
  function run() {
    if (!self.eventsEnable) {
      return;
    }
    const currentValue = self.sound.getBasicValue();
    let diffValue = self.prevValue - currentValue;
    diffValue = diffValue < 0 ? diffValue * -1 : diffValue;
    if (diffValue > 1) {
      self.emit('medicion', currentValue);
      if (self.mqttClient) {
        self.mqttClient.publish(self.myTopic, currentValue.toString());
      }
      self.prevValue = currentValue;
    }
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
