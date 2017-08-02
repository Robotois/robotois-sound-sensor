#ifndef SOUNDWRAPPER_H
#define SOUNDWRAPPER_H

#include <node.h>
#include <node_object_wrap.h>
#include "../SoundSensor.h"

class SoundWrapper : public node::ObjectWrap {
public:
  static void Init();
  static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args);

private:
  explicit SoundWrapper(uint8_t _port = 0x01, uint8_t _addr = 0x00);
  ~SoundWrapper();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void getValue(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void getBasicValue(const v8::FunctionCallbackInfo<v8::Value>& args);
  // static void getScaledValue(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void release(const v8::FunctionCallbackInfo<v8::Value>& args);

  static v8::Persistent<v8::Function> constructor;

  SoundSensor *soundSensor;
};

#endif
