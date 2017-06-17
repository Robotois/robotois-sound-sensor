#include <node.h>
#include "SoundWrapper.h"

using namespace v8;

void CreateObject(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  SoundWrapper::NewInstance(args);
}

void InitAll(Handle<Object> exports, Handle<Object> module) {
  SoundWrapper::Init();
  NODE_SET_METHOD(module,"exports",CreateObject);
}

NODE_MODULE(SoundSensor, InitAll)
