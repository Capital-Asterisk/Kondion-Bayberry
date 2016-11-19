/*
 * FunctionsV8.cpp
 *
 *  Created on: Nov 11, 2016
 *      Author: neal
 */
#include "include/libplatform/libplatform.h"
#include "include/v8.h"

#include "Kondion.h"

using namespace v8;

namespace Kondion {
namespace JS {

class ArrayBufferAllocator;

// I'm not changing these so is it ok to use global vars?

Platform* platform;
Isolate* isolate;
ArrayBufferAllocator* allocator;
Persistent<Context, CopyablePersistentTraits<Context>> contextp;
Persistent<Function, CopyablePersistentTraits<Function>> initialize;

class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
 public:
  virtual void* Allocate(size_t length) {
    void* data = AllocateUninitialized(length);
    return data == NULL ? data : memset(data, 0, length);
  }
  virtual void* AllocateUninitialized(size_t length) {
    return malloc(length);
  }
  virtual void Free(void* data, size_t) {
    free(data);
  }
};

class Bird {
 public:
  uint32_t integrity = 0;
};

void Callback_Kdion_Bird(const v8::FunctionCallbackInfo<v8::Value>& args) {
  //if (args.Length() < 1) return;
  HandleScope handle_scope(isolate);
  //Local<ObjectTemplate> f = ObjectTemplate::New(isolate);
  //f->SetInternalFieldCount(1);
  printf("Bird factory was called!\n");
  if (args.IsConstructCall()) {
    printf("A new bird has been created!\n");
    args.This()->Set(String::NewFromUtf8(isolate, "crushed"),
                     Boolean::New(isolate, false));
    args.This()->SetInternalField(0, External::New(isolate, new Bird()));
    args.GetReturnValue().Set(args.This());
  }
}

void Callback_Bird_GetIntegrity(Local<String> property,
      const PropertyCallbackInfo<Value>& info) {
  Local<External> wrap = Local<External>::Cast(info.Holder()->GetInternalField(0));
  void* pointer = wrap->Value();
  info.GetReturnValue().Set(static_cast<Bird*>(pointer)->integrity);
}

void Callback_Bird_SetIntegrity(Local<String> property,
      Local<Value> value, const PropertyCallbackInfo<void>& info) {
  Local<External> wrap = Local<External>::Cast(info.Holder()->GetInternalField(0));
  void* pointer = wrap->Value();
  static_cast<Bird*>(pointer)->integrity = value->Int32Value();
}


void Callback_Kdion_Log(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() < 1)
    return;
  HandleScope handle_scope(isolate);
  printf("%s\n", *String::Utf8Value(args[0]));
}

void Callback_Kdion_Initialize(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() < 1)
    return;
  HandleScope handle_scope(isolate);
  Local<Value> arg0 = args[0];
  if (arg0->IsFunction()) {
    initialize.Reset();
    initialize = Persistent<Function, CopyablePersistentTraits<Function>>(
        isolate, Local<Function>::Cast(arg0));
  }
}

}
}
