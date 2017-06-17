#include <node.h>
#include "SoundWrapper.h"

using namespace v8;

Persistent<Function> SoundWrapper::constructor;

SoundWrapper::SoundWrapper(uint8_t _port, uint8_t _addr){
  soundSensor = new SoundSensor(_addr);
  soundSensor->selectPort(_port);
}

SoundWrapper::~SoundWrapper(){
}

void SoundWrapper::release(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  SoundWrapper* temp_obj = ObjectWrap::Unwrap<SoundWrapper>(args.Holder());
  temp_obj->soundSensor->release();
  delete temp_obj->soundSensor;
}

void SoundWrapper::Init(){
  Isolate* isolate = Isolate::GetCurrent();
  // Prepare consructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate,New);
  tpl->SetClassName(String::NewFromUtf8(isolate,"SoundWrapper"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl,"getValue",getValue);
  NODE_SET_PROTOTYPE_METHOD(tpl,"getBasicValue",getBasicValue);
  NODE_SET_PROTOTYPE_METHOD(tpl,"release",release);

  constructor.Reset(isolate,tpl->GetFunction());
}

void SoundWrapper::New(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  uint8_t _addr = 0x00, _port = 0x01;
  // If there are two params: First Param => i2c address, second => Port number
  // - Only one Param, this means that the given param is the Port Number,
  // printf("Args Count: %d\n",args.Length());
  SoundWrapper* obj;
  uint8_t _argc = args.Length();
  if(args.IsConstructCall()){
    // Invoked as constructor: `new MyObject(...)`
    switch(_argc){
      case 1: // Only the BCMWrapper is passed
        _port = (uint8_t) args[0]->NumberValue();
        obj = new SoundWrapper(_port);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
        break;
      case 2:
        _port = (uint8_t) args[0]->NumberValue();
        _addr = (uint8_t) args[1]->NumberValue();
        obj = new SoundWrapper(_port,_addr);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
        break;
      default:
        isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments...")));
    }
  }else{
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    if(_argc > 2){
      isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments...")));
    }
    Local<Value>* argv = new Local<Value>[_argc];
    for(uint8_t i = 0; i < _argc; i++){
      argv[i] = args[i];
    }
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(cons->NewInstance(_argc, argv));
  }
}

void SoundWrapper::NewInstance(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  uint8_t _argc = args.Length();
  if(_argc > 2){
    isolate->ThrowException(Exception::TypeError(
    String::NewFromUtf8(isolate, "Wrong arguments...")));
  }
  Handle<Value>* argv = new Handle<Value>[_argc];
  for(uint8_t i = 0; i < _argc; i++){
    argv[i] = args[i];
  }
  Local<Function> cons = Local<Function>::New(isolate, constructor);
  Local<Object> instance = cons->NewInstance(_argc, argv);

  args.GetReturnValue().Set(instance);
}

void SoundWrapper::getValue(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  SoundWrapper* temp_obj = ObjectWrap::Unwrap<SoundWrapper>(args.Holder());

  args.GetReturnValue().Set(Number::New(isolate,temp_obj->soundSensor->getValue()));
}

void SoundWrapper::getBasicValue(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  SoundWrapper* temp_obj = ObjectWrap::Unwrap<SoundWrapper>(args.Holder());

  args.GetReturnValue().Set(Number::New(isolate,temp_obj->soundSensor->getBasicValue()));
}
