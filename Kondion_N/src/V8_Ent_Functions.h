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

void Callback_Entity_Accelerate(
    const FunctionCallbackInfo<v8::Value>& args) {
  if (args.IsConstructCall() || args.Length() == 0)
    return;
  //printf("eggsa");
  if (!(args[0]->IsFloat32Array() || args[0]->IsArray()))
    return;
  //printf("eggs!");
  Local<Array> a = Local<Array>::Cast(args[0]);
  KObj_Entity* pointer_this =
      static_cast<KObj_Entity*>(Local<External>::Cast(
          args.This()->GetInternalField(0))->Value());
  pointer_this->acceleration.x += a->Get(0)->NumberValue();
  pointer_this->acceleration.y += a->Get(1)->NumberValue();
  pointer_this->acceleration.z += a->Get(2)->NumberValue();
  //printf("E\n");
  //const float* b = glm::value_ptr(pointer_this->offset);
  //b[12] = a->Get(0)->NumberValue();
  //b[13] = a->Get(1)->NumberValue();
  //b[14] = a->Get(2)->NumberValue();
  //pointer_this->transform;
}

void Callback_Entity_AddComponent(const FunctionCallbackInfo<Value>& args) {
  if (args.IsConstructCall() || args.Length() == 0)
    return;

  Local<FunctionTemplate> f = Local<FunctionTemplate>::New(isolate,
                                                           p_component);
  //printf("Arg0 is component: %i\n", f->HasInstance(args[0]));

  if (!f->HasInstance(args[0]))
    return;
  KObj_Entity* pointer_this = static_cast<KObj_Entity*>(Local<External>::Cast(
      args.This()->GetInternalField(0))->Value());
  KComponent* pointer_arg0 = static_cast<KComponent*>(Local<External>::Cast(
      Local<Object>::Cast(args[0])->GetInternalField(0))->Value());

  pointer_this->components.push_back(pointer_arg0);
}

void Callback_Entity_PhysLevel(const FunctionCallbackInfo<Value>& args) {
  if (args.IsConstructCall() || args.Length() == 0)
    return;

  KObj_Entity* pointer_this = static_cast<KObj_Entity*>(Local<External>::Cast(
      args.This()->GetInternalField(0))->Value());
  pointer_this->physics = uint8_t(args[0]->NumberValue());
  //KObj_Node::worldObject
  if (pointer_this->getParent() != NULL) {
    if (KObj_Node::all[pointer_this->topObject]->getClass()
        == &KObj::GKO_World::myClass) {
      static_cast<KObj::GKO_World*>(KObj_Node::all[pointer_this->topObject])
          ->terrain.push_back(pointer_this->allIndex);
    }
  }

}

}
}

#endif /* V8_ENT_FUNCTIONS_H_ */
