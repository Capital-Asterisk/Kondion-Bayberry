/*
 * FunctionsV8.cpp
 *
 *  Created on: Nov 11, 2016
 *      Author: neal
 */

#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/libplatform/libplatform.h"
#include "include/v8.h"

#include "Kondion.h"

using namespace v8;

namespace Kondion {
namespace JS {

class ArrayBufferAllocator;

// TODO deal with this mess

ArrayBufferAllocator* allocator;

Platform* platform;
Isolate* isolate;

Persistent<Context, CopyablePersistentTraits<Context>> p_context;
Persistent<Function, CopyablePersistentTraits<Function>> p_initialize;
Persistent<Array, CopyablePersistentTraits<Array>> p_gupdate;
Persistent<Object, CopyablePersistentTraits<Object>> p_input;

Persistent<FunctionTemplate, CopyablePersistentTraits<FunctionTemplate>> p_component;

Persistent<FunctionTemplate, CopyablePersistentTraits<FunctionTemplate>> p_kobj_node;
Persistent<FunctionTemplate, CopyablePersistentTraits<FunctionTemplate>> p_oko_camera;

// kobj js object:
// translate(x, y, z);
// rotate(a, x, y, z);
// dir(in, x, y, z);
// position(in, origin);
// getMatrix(in, what, origin (optional))

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

void Callback_Component(const v8::FunctionCallbackInfo<v8::Value>& args) {
  //if (args.Length() < 1) return;
  HandleScope handle_scope(isolate);

  if (args.IsConstructCall()) {
    //uprintf("A new bird has been created!\n");
    KComponent* o;
    std::string arg = std::string(*String::Utf8Value(args[0]));
    if (arg == "infplane") {
      o = new Component::CPN_InfinitePlane;
    } else {
      o = new Component::CPN_Cube;
    }
    //switch (*String::Utf8Value(args[0]))
    //case "infplane":
    //  o = new Component::CPN_InfinitePlane;
    //  break;
    //default:
    //  o = new Component::CPN_Cube;
    //  break;
    args.This()->SetInternalField(0, External::New(isolate, o));
    args.GetReturnValue().Set(args.This());
  }
}

void Callback_KObj_Entity(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope handle_scope(isolate);
  if (args.IsConstructCall()) {
    printf("New Entity\n");
    KObj_Entity* o = new KObj_Entity();
    o->physics = 1;
    o->components.push_back(new Component::CPN_Cube);
    o->jsObject = new Persistent<v8::Object,
        CopyablePersistentTraits<v8::Object>>(isolate, args.This());
    //Kondion::world.push_back(o);
    args.This()->SetInternalField(0, External::New(isolate, o));
    args.GetReturnValue().Set(args.This());
  }
}

void Callback_GKO_World(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope handle_scope(isolate);
  if (args.IsConstructCall()) {
    printf("New World\n");
    KObj::GKO_World* o = new KObj::GKO_World;
    //o->components.push_back(new Component::CPN_Cube);
    o->name = "World";
    o->jsObject = new Persistent<v8::Object,
        CopyablePersistentTraits<v8::Object>>(isolate, args.This());
    //Kondion::world.push_back(o);
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

void Callback_KObj_SetName(const FunctionCallbackInfo<v8::Value>& args) {
  KObj_Oriented* pointer_this =
          static_cast<KObj_Oriented*>(Local<External>::Cast(
              args.This()->GetInternalField(0))->Value());
  pointer_this->name = std::string(*String::Utf8Value(args[0]));
  //static_cast<Bird*>(pointer)->integrity = value->Int32Value();
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
  Local<FunctionTemplate> f = Local<FunctionTemplate>::New(isolate,
                                                           p_kobj_node);
  printf("Arg0 is kobj_node: %i\n", f->HasInstance(args[0]));

  if (!f->HasInstance(args[0]))
    return;
  KObj_Node* pointer_this = static_cast<KObj_Node*>(Local<External>::Cast(
      args.This()->GetInternalField(0))->Value());
  KObj_Node* pointer_arg0 = static_cast<KObj_Node*>(Local<External>::Cast(
      Local<Object>::Cast(args[0])->GetInternalField(0))->Value());

  pointer_this->setParent(pointer_arg0);
}

void Callback_KObj_World_GetCamera(Local<String> property,
                                   const PropertyCallbackInfo<Value>& info) {
  //Local<External> wrap = Local<External>::Cast(
  //      info.Holder()->GetInternalField(0));
  //void* pointer = wrap->Value();
  if (Renderer::currentCamera == NULL)
    return;
  Persistent<v8::Object, CopyablePersistentTraits<v8::Object>>* p =
      static_cast<Persistent<v8::Object, CopyablePersistentTraits<v8::Object>>*>(Renderer::currentCamera
          ->jsObject);
  Local<v8::Object> o = Local<v8::Object>::New(isolate, *p);
  info.GetReturnValue().Set(o);
}

void Callback_KObj_World_SetCamera(Local<String> property, Local<Value> value,
                                   const PropertyCallbackInfo<void>& info) {
  Local<FunctionTemplate> f = Local<FunctionTemplate>::New(isolate,
                                                           p_oko_camera);
  printf("Arg0 is camera: %i\n", f->HasInstance(value));
  if (!f->HasInstance(value))
    return;
  Local<External> wrap = Local<External>::Cast(
      value->ToObject()->GetInternalField(0));
  void* pointer = wrap->Value();
  Renderer::currentCamera = static_cast<KObj::OKO_Camera_*>(pointer);
  //static_cast<Bird*>(pointer)->integrity = value->Int32Value();
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

void Callback_Kdion_Load(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() < 1)
    return;
  if (!args[0]->IsObject())
    return;
  Local<Object> a = args[0]->ToObject();
  HandleScope handle_scope(isolate);
  // Start loading textures (if any)
  if (a->HasOwnProperty(String::NewFromUtf8(isolate, "textures"))) {
    Local<Value> b = a->Get(String::NewFromUtf8(isolate, "textures"));
    if (b->IsArray()) {
      Local<Array> c = Local<Array>::Cast(b);
      printf("JS: start texture queue\n");
      for (uint16_t i = 0; i < c->Length(); i++) {
        std::string s(*String::Utf8Value(c->Get(i)));
        Resources::Load(s, 1);
      }
    }
  }

  //printf("%s\n", *String::Utf8Value(args[0]));
}

void Callback_Kdion_Log(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() < 1)
    return;
  HandleScope handle_scope(isolate);
  printf("%s\n", *String::Utf8Value(args[0]));
}


void Callback_Kdion_Require(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() < 1)
    return;
  printf("[JS] Running script: %s\n", *String::Utf8Value(args[0]));
  Resources::Raw* f = Resources::Get(std::string(*String::Utf8Value(args[0])));
  std::ostringstream ostring;
  ostring << f->stream->rdbuf();
  Kondion::JS::Eval(ostring.str());

}


}
}
