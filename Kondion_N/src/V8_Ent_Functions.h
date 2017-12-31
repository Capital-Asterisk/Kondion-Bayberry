/*
 * V8_Ent_Functions.h
 *
 *  Created on: Apr 12, 2017
 *      Author: neal
 */

#ifndef V8_ENT_FUNCTIONS_H_
#define V8_ENT_FUNCTIONS_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/libplatform/libplatform.h"
#include "include/v8.h"

#include "Kondion.h"

namespace Kondion {
namespace JS {

// Connects to the Physics::applyForce thing
// applyForce(x, y, z, x, y, z);
void Callback_Entity_ApplyForce(
    const FunctionCallbackInfo<v8::Value>& args) {
    
  // lazy TODO, do this some day
  //if ((!(args[0]->IsFloat32Array() || args[0]->IsArray())))
  //    || (!(args[1]->IsFloat32Array() || args[1]->IsArray())))
  //  return;
    
  Local<Array> a = Local<Array>::Cast(args[0]);
  Local<Array> b = Local<Array>::Cast(args[1]);
  
  KObj_Entity* pointerThis =
      static_cast<KObj_Entity*>(Local<External>::Cast(
          args.This()->GetInternalField(0))->Value());
  
  Physics::ApplyForce(pointerThis, glm::vec3(
      a->Get(0)->NumberValue(),
      a->Get(1)->NumberValue(),
      a->Get(2)->NumberValue()), glm::vec3(
      b->Get(0)->NumberValue(),
      b->Get(1)->NumberValue(),
      b->Get(2)->NumberValue()));
  
  
}

void Callback_Entity_Accelerate(
    const FunctionCallbackInfo<v8::Value>& args) {
  if (args.IsConstructCall() || args.Length() == 0)
    return;
  //printf("eggsa");
  if (!(args[0]->IsFloat32Array() || args[0]->IsArray()))
    return;
  //printf("eggs!");
  Local<Array> a = Local<Array>::Cast(args[0]);
  KObj_Entity* pointerThis =
      static_cast<KObj_Entity*>(Local<External>::Cast(
          args.This()->GetInternalField(0))->Value());
          
  //Physics::applyForce(pointerThis, glm::vec3(a->Get(0)->NumberValue(), 0.0,
  //        a->Get(2)->NumberValue()), glm::vec3(0.0f, -0.004f, 0.0f));
  pointerThis->acceleration.x += a->Get(0)->NumberValue();
  pointerThis->acceleration.y += a->Get(1)->NumberValue();
  pointerThis->acceleration.z += a->Get(2)->NumberValue();

}

void Callback_Entity_AddComponent(const FunctionCallbackInfo<Value>& args) {
  if (args.IsConstructCall() || args.Length() == 0)
    return;

  Local<FunctionTemplate> f = Local<FunctionTemplate>::New(isolate,
                                                           p_component);
  
  // Check if arg0 is a component
  if (!f->HasInstance(args[0]))
    return;
  KObj_Entity* pointerThis = static_cast<KObj_Entity*>(Local<External>::Cast(
      args.This()->GetInternalField(0))->Value());
  KComponent* pointer_arg0 = static_cast<KComponent*>(Local<External>::Cast(
      Local<Object>::Cast(args[0])->GetInternalField(0))->Value());
  pointer_arg0->parent = pointerThis;
  pointerThis->components.push_back(pointer_arg0);
}

void Callback_Entity_GetVelocity(const FunctionCallbackInfo<Value>& args) {
  if (args.IsConstructCall() || args.Length() == 0
      || !(args[0]->IsFloat32Array() || args[0]->IsArray()))
    return;
  KObj_Entity* pointerThis =
          static_cast<KObj_Entity*>(Local<External>::Cast(
              args.This()->GetInternalField(0))->Value());

  Local<Float32Array> a = Local<Float32Array>::Cast(args[0]);
  a->Set(0, Number::New(isolate, pointerThis->velocity.x));
  a->Set(1, Number::New(isolate, pointerThis->velocity.y));
  a->Set(2, Number::New(isolate, pointerThis->velocity.z));
  //pointerThis->offset[0][0];
  //static_cast<Bird*>(pointer)->integrity = value->Int32Value();
}

void Callback_Entity_SetVelocity(const FunctionCallbackInfo<Value>& args) {
  if (args.IsConstructCall() || args.Length() == 0
      || !(args[0]->IsFloat32Array() || args[0]->IsArray()))
    return;

  KObj_Entity* pointerThis =
          static_cast<KObj_Entity*>(Local<External>::Cast(
              args.This()->GetInternalField(0))->Value());

  Local<Float32Array> a = Local<Float32Array>::Cast(args[0]);

  pointerThis->velocity.x = a->Get(0)->NumberValue();
  pointerThis->velocity.y = a->Get(1)->NumberValue();
  pointerThis->velocity.z = a->Get(2)->NumberValue();
}

void Callback_Renderable_SetMaterial(const FunctionCallbackInfo<Value>& args) {
  if (args.IsConstructCall() || args.Length() == 0)
    return;

  Local<FunctionTemplate> f = Local<FunctionTemplate>::New(isolate,
                                                           p_material);
  // Check if argument is a material
  if (!f->HasInstance(args[0]))
    return;
  KObj_Renderable* pointerThis = static_cast<KObj_Entity*>(Local<External>::Cast(
      args.This()->GetInternalField(0))->Value());
  KMaterial* pointer_arg0 = static_cast<KMaterial*>(Local<External>::Cast(
      Local<Object>::Cast(args[0])->GetInternalField(0))->Value()); 
  uint16_t i = 0;
  //printf("pointer: %p\n", pointerThis);
  pointerThis->material = uint16_t(-1);
  while (pointerThis->material == uint16_t(-1) && i != KMaterial::materials.size()) {
    if (KMaterial::materials[i] == pointer_arg0)
      pointerThis->material = i;
    i ++;
  }
  
}

void Callback_Entity_PhysLevel(const FunctionCallbackInfo<Value>& args) {
  if (args.IsConstructCall() || args.Length() == 0)
    return;

  KObj_Entity* pointerThis = static_cast<KObj_Entity*>(Local<External>::Cast(
      args.This()->GetInternalField(0))->Value());
  pointerThis->physics = uint8_t(args[0]->NumberValue());
  //KObj_Node::worldObject
  if (pointerThis->getParent() != NULL) {
    if (KObj_Node::all[pointerThis->topObject]->getClass()
        == KObj::GKO_World::myClass) {
      static_cast<KObj::GKO_World*>(KObj_Node::all[pointerThis->topObject])
          ->terrain.push_back(pointerThis->allIndex);
    }
  }

}

void Callback_KObj_Node_GetOnupdate(Local<String> property,
                                   const PropertyCallbackInfo<Value>& info) {
  KObj_Node* pointerThis = static_cast<KObj_Node*>(Local<External>::Cast(
      info.Holder()->GetInternalField(0))->Value());

  Persistent<Array, CopyablePersistentTraits<Array>>* p;
  p = static_cast<Persistent<Array, CopyablePersistentTraits<Array>>*>
        (pointerThis->jsHidden);
  Local<Array> a = Local<Array>::New(isolate, *p);
  info.GetReturnValue().Set(a->Get(0));
}

void Callback_KObj_Node_SetOnupdate(Local<String> property, Local<Value> value,
                                   const PropertyCallbackInfo<void>& info) {

  if (!value->IsFunction())
    return;

  KObj_Node* pointerThis = static_cast<KObj_Node*>(Local<External>::Cast(
      info.Holder()->GetInternalField(0))->Value());

  Persistent<Array, CopyablePersistentTraits<Array>>* p;
  p = static_cast<Persistent<Array, CopyablePersistentTraits<Array>>*>
        (pointerThis->jsHidden);
  Local<Array> a = Local<Array>::New(isolate, *p);
  a->Set(0, value);
  
  KObj_Node::worldObject->jsUpdate.push_back(pointerThis->allIndex);
}

void Callback_KObj_Entity_GetOncollide(Local<String> property,
                                   const PropertyCallbackInfo<Value>& info) {
  KObj_Node* pointerThis = static_cast<KObj_Node*>(Local<External>::Cast(
      info.Holder()->GetInternalField(0))->Value());

  Persistent<Array, CopyablePersistentTraits<Array>>* p;
  p = static_cast<Persistent<Array, CopyablePersistentTraits<Array>>*>
        (pointerThis->jsHidden);
  Local<Array> a = Local<Array>::New(isolate, *p);
  info.GetReturnValue().Set(a->Get(1));
}

void Callback_KObj_Entity_SetOncollide(Local<String> property, Local<Value> value,
                                   const PropertyCallbackInfo<void>& info) {

  if (!value->IsFunction())
    return;

  KObj_Node* pointerThis = static_cast<KObj_Node*>(Local<External>::Cast(
      info.Holder()->GetInternalField(0))->Value());

  Persistent<Array, CopyablePersistentTraits<Array>>* p;
  p = static_cast<Persistent<Array, CopyablePersistentTraits<Array>>*>
        (pointerThis->jsHidden);
  Local<Array> a = Local<Array>::New(isolate, *p);
  a->Set(1, value);

}

}
}

#endif /* V8_ENT_FUNCTIONS_H_ */
