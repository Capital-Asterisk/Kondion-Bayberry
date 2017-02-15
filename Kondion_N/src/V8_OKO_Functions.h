/*
 * V8_OKO_Functions.h
 *
 *  Created on: Feb 13, 2017
 *      Author: neal
 */

#ifndef V8_OKO_FUNCTIONS_H_
#define V8_OKO_FUNCTIONS_H_

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

void Callback_OKO_Camera(const FunctionCallbackInfo<v8::Value>& args) {
  if (!args.IsConstructCall())
    return;
  HandleScope handle_scope(isolate);
  printf("New Camera\n");
  KObj::OKO_Camera_* o = new KObj::OKO_Camera_();
  //o->components.push_back(new Component::CPN_Cube);
  o->jsObject = new Persistent<v8::Object,
      CopyablePersistentTraits<v8::Object>>(isolate, args.This());
  o->name = "Camera";
  //Kondion::world.push_back(o);
  args.This()->SetInternalField(0, External::New(isolate, o));
  args.GetReturnValue().Set(args.This());
}

void Callback_OKO_GetOffsetPosition(
    const FunctionCallbackInfo<v8::Value>& args) {
  if (args.IsConstructCall() || args.Length() == 0
      || !(args[0]->IsFloat32Array() || args[0]->IsArray()))
    return;
  KObj_Oriented* pointer_this =
          static_cast<KObj_Oriented*>(Local<External>::Cast(
              args.This()->GetInternalField(0))->Value());
  Local<Array> a = Local<Array>::Cast(args[0]);
  a->Set(0, Number::New(isolate, pointer_this->orientation[3][0]));
  a->Set(1, Number::New(isolate, pointer_this->orientation[3][1]));
  a->Set(2, Number::New(isolate, pointer_this->orientation[3][2]));
  //const float* b = glm::value_ptr(pointer_this->offset);
  //b[12] = a->Get(0)->NumberValue();
  //b[13] = a->Get(1)->NumberValue();
  //b[14] = a->Get(2)->NumberValue();
  //pointer_this->transform;
}

void Callback_OKO_SetOffsetPosition(
    const FunctionCallbackInfo<v8::Value>& args) {
  if (args.IsConstructCall() || args.Length() == 0)
    return;
  //printf("eggsa");
  if (!(args[0]->IsFloat32Array() || args[0]->IsArray()))
    return;
  //printf("eggs!");
  Local<Array> a = Local<Array>::Cast(args[0]);
  KObj_Oriented* pointer_this =
      static_cast<KObj_Oriented*>(Local<External>::Cast(
          args.This()->GetInternalField(0))->Value());
  pointer_this->orientation[3][0] = a->Get(0)->NumberValue();
  pointer_this->orientation[3][1] = a->Get(1)->NumberValue();
  pointer_this->orientation[3][2] = a->Get(2)->NumberValue();
  //const float* b = glm::value_ptr(pointer_this->offset);
  //b[12] = a->Get(0)->NumberValue();
  //b[13] = a->Get(1)->NumberValue();
  //b[14] = a->Get(2)->NumberValue();
  //pointer_this->transform;
}

void Callback_OKO_Translate(const FunctionCallbackInfo<v8::Value>& args) {
  if (args.IsConstructCall() || args.Length() < 3)
    return;
  Local<Array> a = Local<Array>::Cast(args[0]);
  KObj_Oriented* pointer_this =
      static_cast<KObj_Oriented*>(Local<External>::Cast(
          args.This()->GetInternalField(0))->Value());
  pointer_this->orientation = glm::translate(
      pointer_this->orientation,
      glm::vec3(args[0]->NumberValue(), args[1]->NumberValue(),
                args[2]->NumberValue()));
  //pointer_this->offset[3][0] = a->Get(0)->NumberValue();
  //pointer_this->offset[3][1] = a->Get(1)->NumberValue();
  //pointer_this->offset[3][2] = a->Get(2)->NumberValue();
}

void Callback_OKO_PointAt(const FunctionCallbackInfo<v8::Value>& args) {
  //// x, y, z, up x, up y, up z, origin
  // at, up x, up y, up z, origin
  if (args.IsConstructCall())
    return;

  KObj_Oriented* pointer_this =
      static_cast<KObj_Oriented*>(Local<External>::Cast(
          args.This()->GetInternalField(0))->Value());
  // point at the oriented object on the first argument
  KObj_Oriented* pointer_that =
      static_cast<KObj_Oriented*>(Local<External>::Cast(
          args[0]->ToObject()->GetInternalField(0))->Value());

  // TODO: set these wwith arguments
  glm::mat4 origin(1);
  glm::vec4 up(0.0f, 1.0f, 0.0f, 0.0f);

  // Remove translation from origin
  origin[3][0] = 0;
  origin[3][1] = 0;
  origin[3][2] = 0;

  // TODO: Do something to origin to use "up"

  // Multiply both by the origin
  pointer_this->orientation = origin * pointer_this->orientation;
  glm::mat4 that = origin * pointer_that->orientation;

  // TODO account for scale

  // Create a rotation that points towards the target
  glm::quat f(
      glm::vec3(
          -glm::atan(
              (pointer_this->orientation[3][1] - that[3][1])
                  / glm::length(
                      glm::vec2(pointer_this->orientation[3][0] - that[3][0],
                                pointer_this->orientation[3][2] - that[3][2]))),
          glm::atan(pointer_this->orientation[3][0] - that[3][0],
                    pointer_this->orientation[3][2] - that[3][2]),
          0.0f));
  // Then convert it to Mat4
  glm::mat4 abird = glm::toMat4(f);

  // Set the new matrix's translation to this object's translation
  abird[3][0] = pointer_this->orientation[3][0];
  abird[3][1] = pointer_this->orientation[3][1];
  abird[3][2] = pointer_this->orientation[3][2];

  // set this translation to the new one
  pointer_this->orientation = abird;

  // inverse the origin
  glm::inverse(origin);

  // Multiply by the inverse to undo the effects of the first origin multiplication.
  pointer_this->orientation = origin * pointer_this->orientation;

  //glm::vec4 direction(0, 0, -1, 0);
  //direction = pointer_this->offset * direction;
  //direction = glm::normalize(direction);

  //glm::vec3 difference(pointer_that->offset[3][0],
  //                       pointer_that->offset[3][1], pointer_that->offset[3][2]);
  //difference -= glm::vec3(pointer_this->offset[3][0],
  //                        pointer_this->offset[3][1], pointer_this->offset[3][2]);
  //difference = glm::normalize(difference);

  //glm::quat f = glm::rotation(glm::vec3(direction), difference);
  //glm::rotate(f, 1.2f, glm::vec3(0.0f, 0.0f, 1.0f));
  //pointer_this->offset *= glm::toMat4(f);

  //pointer_this->offset = glm::rotate(pointer_this->offset, 0.1f, glm::vec3(0.0f, 0.0f, 1.0f));

  ////direction = up;
  ////direction = pointer_this->offset * direction;
  ////direction = glm::normalize(direction);

  //difference *= 0.02;
  //pointer_this->offset[3][0] += difference.x;
  //pointer_this->offset[3][1] += difference.y;
  //pointer_this->offset[3][2] += difference.z;
  //Debug::printMatrix(pointer_this->orientation);

  //pointer_this->offset = glm::translate(pointer_this->offset, glm::vec3(
  //    args[0]->NumberValue(),
  //    args[1]->NumberValue(),
  //    args[2]->NumberValue()));
  //pointer_this->offset[3][0] = a->Get(0)->NumberValue();
  //pointer_this->offset[3][1] += 0.02f;
  //pointer_this->offset[3][2] = a->Get(2)->NumberValue();
}

void Callback_OKO_Rotate(const FunctionCallbackInfo<v8::Value>& args) {
  if (args.IsConstructCall() || args.Length() == 0)
    return;
  if (!args[0]->IsArray())
    return;
  Local<Array> a = Local<Array>::Cast(args[0]);
  KObj_Oriented* pointer_this =
      static_cast<KObj_Oriented*>(Local<External>::Cast(
          args.This()->GetInternalField(0))->Value());
  pointer_this->orientation[3][0] = a->Get(0)->NumberValue();
  pointer_this->orientation[3][1] = a->Get(1)->NumberValue();
  pointer_this->orientation[3][2] = a->Get(2)->NumberValue();
}

}
}

#endif /* V8_OKO_FUNCTIONS_H_ */
