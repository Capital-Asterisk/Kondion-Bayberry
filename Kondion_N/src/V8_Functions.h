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
Persistent<Object, CopyablePersistentTraits<Object>> p_inputd;

Persistent<FunctionTemplate, CopyablePersistentTraits<FunctionTemplate>> p_component;
Persistent<FunctionTemplate, CopyablePersistentTraits<FunctionTemplate>> p_material;

Persistent<FunctionTemplate, CopyablePersistentTraits<FunctionTemplate>> p_kobj_node;
Persistent<FunctionTemplate, CopyablePersistentTraits<FunctionTemplate>> p_oko_camera;
Persistent<FunctionTemplate, CopyablePersistentTraits<FunctionTemplate>> p_sky;

// kobj js object:
// translate(x, y, z);
// rotate(a, x, y, z);
// dir(in, x, y, z);
// position(in, origin);
// getMatrix(in, what, origin (optional))

class ArrayBufferAllocator : public ArrayBuffer::Allocator {
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

void Callback_Kdion_Bird(const FunctionCallbackInfo<Value>& args) {
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

void Callback_Component(const FunctionCallbackInfo<Value>& args) {
  //if (args.Length() < 1) return;
  HandleScope handle_scope(isolate);

  if (args.IsConstructCall()) {
    //uprintf("A new bird has been created!\n");
    KComponent* o;
    std::string arg = std::string(*String::Utf8Value(args[0]));
    if (arg == "infplane") {
      o = new Component::CPN_InfinitePlane;
    } else if (arg == "sphere") {
      o = new Component::CPN_Sphere;
    } else  {
      o = new Component::CPN_Cube;
    }
    o->jsObject = new Persistent<Object,
        CopyablePersistentTraits<Object>>(isolate, args.This());

    args.This()->SetInternalField(0, External::New(isolate, o));
    args.GetReturnValue().Set(args.This());
  }
}

void Callback_Component_GetMatrix(const FunctionCallbackInfo<Value>& args) {
  if (args.IsConstructCall() || args.Length() == 0
      || !(args[0]->IsFloat32Array() || args[0]->IsArray()))
    return;
  KComponent* pointer_this =
          static_cast<KComponent*>(Local<External>::Cast(
              args.This()->GetInternalField(0))->Value());
  Debug::printMatrix(pointer_this->offset);

  Local<Float32Array> a = Local<Float32Array>::Cast(args[0]);
  a->Set(0, Number::New(isolate, pointer_this->offset[3][0]));
  a->Set(1, Number::New(isolate, pointer_this->offset[3][1]));
  a->Set(2, Number::New(isolate, pointer_this->offset[3][2]));
  //pointer_this->offset[0][0];
  //static_cast<Bird*>(pointer)->integrity = value->Int32Value();
}

void Callback_Component_SetMatrix(const FunctionCallbackInfo<Value>& args) {
  if (args.IsConstructCall() || args.Length() == 0
      || !(args[0]->IsFloat32Array() || args[0]->IsArray()))
    return;

  KComponent* pointer_this =
          static_cast<KComponent*>(Local<External>::Cast(
              args.This()->GetInternalField(0))->Value());

  Local<Float32Array> a = Local<Float32Array>::Cast(args[0]);

  //const float* b = glm::value_ptr(pointer_this->offset);
  //for (uint8_t i = 0; i < 16; i ++)
  //  b[i] = a->Get(i)->NumberValue();

  // TODO: make this more efficient or something

  pointer_this->offset[0][0] = a->Get(0)->NumberValue();
  pointer_this->offset[0][1] = a->Get(1)->NumberValue();
  pointer_this->offset[0][2] = a->Get(2)->NumberValue();
  pointer_this->offset[0][3] = a->Get(3)->NumberValue();

  pointer_this->offset[1][0] = a->Get(4)->NumberValue();
  pointer_this->offset[1][1] = a->Get(5)->NumberValue();
  pointer_this->offset[1][2] = a->Get(6)->NumberValue();
  pointer_this->offset[1][3] = a->Get(7)->NumberValue();

  pointer_this->offset[2][0] = a->Get(8)->NumberValue();
  pointer_this->offset[2][1] = a->Get(9)->NumberValue();
  pointer_this->offset[2][2] = a->Get(10)->NumberValue();
  pointer_this->offset[2][3] = a->Get(11)->NumberValue();

  pointer_this->offset[3][0] = a->Get(12)->NumberValue();
  pointer_this->offset[3][1] = a->Get(13)->NumberValue();
  pointer_this->offset[3][2] = a->Get(14)->NumberValue();
  pointer_this->offset[3][3] = a->Get(15)->NumberValue();

  //printf("New matrix: \n");
  //Debug::printMatrix(pointer_this->offset);
  //static_cast<Bird*>(pointer)->integrity = value->Int32Value();
}

void Callback_Material(const FunctionCallbackInfo<Value>& args) {
  //if (args.Length() < 1) return;
  HandleScope handle_scope(isolate);

  if (args.IsConstructCall()) {
    std::string arg = std::string(*String::Utf8Value(args[0]));
    KMaterial* o = new KMaterial;

    for (uint16_t i = 0; i < Resources::KShader::shaders.size(); i ++) {
      if (arg == Resources::KShader::shaders[i]->identifier) {
        //printf("Match: %s\n", Resources::KShader::shaders[i]->identifier.c_str());
        o->shader = i;
      }
    }
    
    args.This()->SetInternalField(0, External::New(isolate, o));
    args.GetReturnValue().Set(args.This());
  }
}

void Callback_Material_GetId(const FunctionCallbackInfo<Value>& args) {
  
  
}

void Callback_Material_GetUniform(const FunctionCallbackInfo<Value>& args) {
  
}

void Callback_Material_SetUniform(const FunctionCallbackInfo<Value>& args) {
  if (args.IsConstructCall() || args.Length() < 2 || !args[0]->IsNumber())
    return;
  
  KMaterial* pointerThis =
      static_cast<KMaterial*>(Local<External>::Cast(
        args.This()->GetInternalField(0))->Value());
        
  uint16_t index = args[0]->NumberValue();
  
  // KShader::shaders[pointerThis->shader]
  Resources::KShader* shader = Resources::KShader::shaders[pointerThis->shader];
  
  if (shader->uniformCount <= index) 
    return;
    
 
  
  switch (shader->uniformTypes[index]) {
    case 0: // int
      (*static_cast<int32_t*>(pointerThis->uniforms[index])) = uint32_t(args[1]->NumberValue());
    case 1: // uint
      (*static_cast<uint32_t*>(pointerThis->uniforms[index])) = int32_t(args[1]->NumberValue());
      break;
    case 10: // float
      (*static_cast<float*>(pointerThis->uniforms[index])) = float(args[1]->NumberValue());
      break;
    case 11: // double
      (*static_cast<double*>(pointerThis->uniforms[index])) = args[1]->NumberValue();
      break;
    case 30: // texture
      if (args[0]->IsNumber())
        (*static_cast<int16_t*>(pointerThis->uniforms[index])) = uint16_t(args[1]->NumberValue());
      
      break;
  }
  
  //for (uint16_t i = 0; i < shader->uniformCount; i ++)
  
  
  
}




void Callback_KObj_Entity(const FunctionCallbackInfo<Value>& args) {
  HandleScope handle_scope(isolate);
  if (args.IsConstructCall()) {
    //printf("NEW: Entity\n");
    KObj_Entity* o = new KObj_Entity();
    o->jsObject = new Persistent<Object,
        CopyablePersistentTraits<Object>>(isolate, args.This());
    //Kondion::world.push_back(o);
    args.This()->SetInternalField(0, External::New(isolate, o));
    args.GetReturnValue().Set(args.This());
  }
}

void Callback_Kdion_Blank(const FunctionCallbackInfo<Value>& args) {
  //HandleScope handle_scope(isolate);
  if (args.IsConstructCall()) {
    //KObj_Entity* f = new KObj_Entity();
    //f->components.push_back(new Component::CPN_Cube);
    //Kondion::world.push_back(f);
    //args.This()->SetInternalField(0, External::New(isolate, f));
    args.GetReturnValue().Set(args.This());
  }
}

void Callback_KObj_SetName(const FunctionCallbackInfo<Value>& args) {
  KObj_Oriented* pointer_this =
          static_cast<KObj_Oriented*>(Local<External>::Cast(
              args.This()->GetInternalField(0))->Value());
  pointer_this->name = std::string(*String::Utf8Value(args[0]));
  //static_cast<Bird*>(pointer)->integrity = value->Int32Value();
}

void Callback_KObj_GetParent(const FunctionCallbackInfo<Value>& args) {
  //HandleScope handle_scope(isolate);
  if (!args.IsConstructCall()) {

    Local<External> wrap = Local<External>::Cast(
        args.This()->GetInternalField(0));
    KObj_Node* pointer = static_cast<KObj_Node*>(wrap->Value());
    Persistent<Object, CopyablePersistentTraits<Object>>* p;
    p =
        static_cast<Persistent<Object, CopyablePersistentTraits<Object>>*>(pointer
            ->jsObject);
    Local<Object> o = Local<Object>::New(isolate, *p);
    args.GetReturnValue().Set(o);
  }
}

void Callback_KObj_SetParent(const FunctionCallbackInfo<Value>& args) {
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
  //Persistent<Object, CopyablePersistentTraits<Object>>* p;
  //p = static_cast<Persistent<Object, CopyablePersistentTraits<Object>>*>(pointer->jsObject);
  //Local<Object> o = Local<Object>::New(isolate, *p);
  //args.GetReturnValue().Set(o);
  Local<FunctionTemplate> f = Local<FunctionTemplate>::New(isolate,
                                                           p_kobj_node);
  //printf("Arg0 is kobj_node: %i\n", f->HasInstance(args[0]));

  if (!f->HasInstance(args[0]))
    return;
  KObj_Node* pointer_this = static_cast<KObj_Node*>(Local<External>::Cast(
      args.This()->GetInternalField(0))->Value());
  KObj_Node* pointer_arg0 = static_cast<KObj_Node*>(Local<External>::Cast(
      Local<Object>::Cast(args[0])->GetInternalField(0))->Value());

  pointer_this->setParent(pointer_arg0);
}

void Callback_GKO_World(const FunctionCallbackInfo<Value>& args) {
  HandleScope handle_scope(isolate);
  if (args.IsConstructCall()) {
    //printf("NEW: World\n");
    KObj::GKO_World* o = new KObj::GKO_World;
    //o->components.push_back(new Component::CPN_Cube);
    o->name = "World";
    o->jsObject = new Persistent<Object,
        CopyablePersistentTraits<Object>>(isolate, args.This());
    //Kondion::world.push_back(o);
    args.This()->SetInternalField(0, External::New(isolate, o));
    args.GetReturnValue().Set(args.This());
  }
}

void Callback_RKO_Sky(const FunctionCallbackInfo<Value>& args) {
  HandleScope handle_scope(isolate);
  if (args.IsConstructCall()) {
    //printf("NEW: World\n");
    KObj::RKO_Sky* o = new KObj::RKO_Sky;
    //o->components.push_back(new Component::CPN_Cube);
    o->name = "Sky";
    o->jsObject = new Persistent<Object,
        CopyablePersistentTraits<Object>>(isolate, args.This());
    //Kondion::world.push_back(o);
    args.This()->SetInternalField(0, External::New(isolate, o));
    args.GetReturnValue().Set(args.This());
  }
}

void Callback_KObj_World_GetCamera(Local<String> property,
                                   const PropertyCallbackInfo<Value>& info) {
  //Local<External> wrap = Local<External>::Cast(
  //      info.Holder()->GetInternalField(0));
  //void* pointer = wrap->Value();
  if (Renderer::currentCamera == NULL)
    return;
  Persistent<Object, CopyablePersistentTraits<Object>>* p =
      static_cast<Persistent<Object, CopyablePersistentTraits<Object>>*>(
          Renderer::currentCamera->jsObject);
  Local<Object> o = Local<Object>::New(isolate, *p);
  info.GetReturnValue().Set(o);
}

void Callback_KObj_World_SetCamera(Local<String> property, Local<Value> value,
                                   const PropertyCallbackInfo<void>& info) {
  Local<FunctionTemplate> f = Local<FunctionTemplate>::New(isolate,
                                                           p_oko_camera);
  //printf("Arg0 is camera: %i\n", f->HasInstance(value));
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

void Callback_Kdion_Delta(Local<String> property,
                                   const PropertyCallbackInfo<Value>& info) {
  info.GetReturnValue().Set(Number::New(isolate, Kondion::Delta()));
}


void Callback_Kdion_GlobalUpdate(
    const FunctionCallbackInfo<Value>& args) {
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
    const FunctionCallbackInfo<Value>& args) {
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

void Callback_Kdion_Load(const FunctionCallbackInfo<Value>& args) {
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
      //printf("JS: start texture queue\n");
      for (uint16_t i = 0; i < c->Length(); i++) {
        std::string s(*String::Utf8Value(c->Get(i)));
        Resources::Load(s, 1);
      }
    }
  }
  
  // Start loading shaders (if any)
  if (a->HasOwnProperty(String::NewFromUtf8(isolate, "materials"))) {
    Local<Value> b = a->Get(String::NewFromUtf8(isolate, "materials"));
    if (b->IsArray()) {
      Local<Array> c = Local<Array>::Cast(b);
      //printf("JS: start texture queue\n");
      bool loaded = false;
      for (uint16_t i = 0; i < c->Length(); i++) {
        std::string s(*String::Utf8Value(c->Get(i)));
        for (uint16_t i = 0; i < Resources::KShader::shaders.size() && !loaded; i ++) {
          if (Resources::KShader::shaders[i]->identifier == s) {
            Resources::KShader::shaders[i]->Load(true);
            loaded = false;
          }
        }
      }
    }
  }

  //printf("%s\n", *String::Utf8Value(args[0]));
}

void Callback_Kdion_Log(const FunctionCallbackInfo<Value>& args) {
  if (args.Length() < 1)
    return;
  HandleScope handle_scope(isolate);
  printf("[JS Log] %s\n", *String::Utf8Value(args[0]));
}


void Callback_Kdion_Require(const FunctionCallbackInfo<Value>& args) {
  if (args.Length() < 1)
    return;
  printf("[JS] Running script: %s\n", *String::Utf8Value(args[0]));
  Resources::Raw* f = Resources::Get(std::string(*String::Utf8Value(args[0])));
  //std::ostringstream ostring;
  //ostring << f->stream->rdbuf();
  std::string s;
  f->stringy(s);
  Kondion::JS::Eval(s);
  delete f;
}


}
}
