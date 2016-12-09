/*
 * FunctionsV8.cpp
 *
 *  Created on: Nov 11, 2016
 *      Author: neal
 */

#include <glm/gtc/type_ptr.hpp>

#include "include/libplatform/libplatform.h"
#include "include/v8.h"

#include "Kondion.h"

using namespace v8;

namespace Kondion {
namespace JS {

class ArrayBufferAllocator;

// Is this a bad thing?

ArrayBufferAllocator* allocator;

Platform* platform;
Isolate* isolate;

Persistent<Context, CopyablePersistentTraits<Context>> p_context;
Persistent<Function, CopyablePersistentTraits<Function>> p_initialize;
Persistent<Array, CopyablePersistentTraits<Array>> p_gupdate;
Persistent<Object, CopyablePersistentTraits<Object>> p_input;

Persistent<FunctionTemplate, CopyablePersistentTraits<FunctionTemplate>> p_kobj_node;


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

void Callback_KObj_Entity(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope handle_scope(isolate);
  if (args.IsConstructCall()) {
    printf("New Entity\n");
    KObj_Entity* o = new KObj_Entity();
    o->components.push_back(new Component::CPN_Cube);
    o->jsObject = new Persistent<v8::Object,
        CopyablePersistentTraits<v8::Object>>(isolate, args.This());
    Kondion::world.push_back(o);
    args.This()->SetInternalField(0, External::New(isolate, o));
    args.GetReturnValue().Set(args.This());
  }
}

void Callback_GKO_World(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope handle_scope(isolate);
  if (args.IsConstructCall()) {
    printf("New Entity\n");
    KObj::GKO_World* o = new KObj::GKO_World();
    //o->components.push_back(new Component::CPN_Cube);
    o->jsObject = new Persistent<v8::Object,
        CopyablePersistentTraits<v8::Object>>(isolate, args.This());
    Kondion::world.push_back(o);
    args.This()->SetInternalField(0, External::New(isolate, o));
    args.GetReturnValue().Set(args.This());
  }
}

void Callback_Kdion_Blank(const v8::FunctionCallbackInfo<v8::Value>& args) {
  //HandleScope handle_scope(isolate);
  if (args.IsConstructCall()) {
    //KObj_Entity* f = new KObj_Entity();
    //f->components.push_back(new Component::CPN_Cube);
    //Kondion::world.push_back(f);
    //args.This()->SetInternalField(0, External::New(isolate, f));
    args.GetReturnValue().Set(args.This());
  }
}

void Callback_KObj_GetParent(const v8::FunctionCallbackInfo<v8::Value>& args) {
  //HandleScope handle_scope(isolate);
  if (!args.IsConstructCall()) {
    //printf("%s\n", static_cast<Persistent<v8::Object, CopyablePersistentTraits<v8::Object>>>(static_cast<KObj_Node*>(args.This()->GetInternalField(0)->Value())->getParent()->jsObject)->name);
    Local<External> wrap = Local<External>::Cast(
        args.This()->GetInternalField(0));
    KObj_Node* pointer = static_cast<KObj_Node*>(wrap->Value());
    Persistent<v8::Object, CopyablePersistentTraits<v8::Object>>* p;
    p =
        static_cast<Persistent<v8::Object, CopyablePersistentTraits<v8::Object>>*>(pointer
            ->jsObject);
    Local<v8::Object> o = Local<v8::Object>::New(isolate, *p);
    args.GetReturnValue().Set(o);
  }
}

void Callback_KObj_SetParent(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.IsConstructCall() || args.Length() == 0)
    return;
  // A mess
  //Local<FunctionTemplate> f;
  //Local<FunctionTemplate> f = args.GetIsolate()->GetCallingContext()->Global()->Get(
  //    String::NewFromUtf8(isolate, "KObj_Node"));
  //Local<Function> b = args.GetIsolate()->GetCallingContext()->Global()->Get(
  //        String::NewFromUtf8(isolate, "KObj_Node"));
  //args[0].Cast(f);
  //if (!Local<FunctionTemplate>::Cast(
  //    args.GetIsolate()->GetCallingContext()->Global()->Get(
  //        String::NewFromUtf8(isolate, "KObj_Node")))->HasInstance(args[0]))
  //  // Test if arg[0] is a kobj_node
  //  return;
  //KObj_Node* pointer = static_cast<KObj_Node*>(Local<External>::Cast(
  //    args.This()->GetInternalField(0))->Value());
  //printf("args: %i\n", args.Length());
  //pointer->setParent(node);
  //Persistent<v8::Object, CopyablePersistentTraits<v8::Object>>* p;
  //p = static_cast<Persistent<v8::Object, CopyablePersistentTraits<v8::Object>>*>(pointer->jsObject);
  //Local<v8::Object> o = Local<v8::Object>::New(isolate, *p);
  //args.GetReturnValue().Set(o);
  Local<FunctionTemplate> f = Local<FunctionTemplate>::New(isolate, p_kobj_node);
  printf("Arg0 is kobj_node: %i\n", f->HasInstance(args[0]));

  if (!f->HasInstance(args[0]))
    return;
  KObj_Node* pointer_this = static_cast<KObj_Node*>(Local<External>::Cast(
        args.This()->GetInternalField(0))->Value());
  KObj_Node* pointer_arg0 = static_cast<KObj_Node*>(Local<External>::Cast(
        Local<Object>::Cast(args[0])->GetInternalField(0))->Value());

  pointer_this->setParent(pointer_arg0);
}

void Callback_Bird_GetIntegrity(Local<String> property,
                                const PropertyCallbackInfo<Value>& info) {
  Local<External> wrap = Local<External>::Cast(
      info.Holder()->GetInternalField(0));
  void* pointer = wrap->Value();
  info.GetReturnValue().Set(static_cast<Bird*>(pointer)->integrity);
}

void Callback_Bird_SetIntegrity(Local<String> property, Local<Value> value,
                                const PropertyCallbackInfo<void>& info) {
  Local<External> wrap = Local<External>::Cast(
      info.Holder()->GetInternalField(0));
  void* pointer = wrap->Value();
  static_cast<Bird*>(pointer)->integrity = value->Int32Value();
}

void Callback_Oriented_SetOffsetPosition(const FunctionCallbackInfo<v8::Value>& args) {
  if (args.IsConstructCall() || args.Length() == 0)
    return;
  if (!args[0]->IsArray())
    return;
  Local<Array> a = Local<Array>::Cast(args[0]);
  KObj_Oriented* pointer_this = static_cast<KObj_Oriented*>(Local<External>::Cast(
        args.This()->GetInternalField(0))->Value());
  pointer_this->offset[3][0] = a->Get(0)->NumberValue();
  pointer_this->offset[3][1] = a->Get(1)->NumberValue();
  pointer_this->offset[3][2] = a->Get(2)->NumberValue();
  //const float* b = glm::value_ptr(pointer_this->offset);
  //b[12] = a->Get(0)->NumberValue();
  //b[13] = a->Get(1)->NumberValue();
  //b[14] = a->Get(2)->NumberValue();
  //pointer_this->transform;
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
    p_initialize.Reset();
    p_initialize = Persistent<Function, CopyablePersistentTraits<Function>>(
        isolate, Local<Function>::Cast(arg0));
  }
}

void Callback_Kdion_GlobalUpdate(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() < 1)
    return;
  HandleScope handle_scope(isolate);
  Local<Value> arg0 = args[0];
  if (arg0->IsFunction()) {
    Local<Array> a = Local<Array>::New(isolate, p_gupdate);
    a->Set(a->Length(), arg0);
  }
}

}
}
