/*
 * KondionV8.cpp
 *
 *  Created on: Oct 30, 2016
 *      Author: neal
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include <include/libplatform/libplatform.h>
#include <include/v8.h>

#include "Kondion.h"
#include "FunctionsV8.h"

using namespace v8;

namespace Kondion {
namespace JS {

namespace ON {

std::vector<Persistent<Value, CopyablePersistentTraits<Value>>*> objects;

int Parse(const std::string& s) {
  Isolate::Scope isolate_scope(isolate);
  HandleScope handle_scope(isolate);
  Local<Context> context = Local<Context>::New(isolate, p_context);
  Context::Scope context_scope(context);
  printf("%s", s.c_str());
  Local<String> str =
      String::NewFromUtf8(isolate, s.c_str(), NewStringType::kNormal)
          .ToLocalChecked();
  //Local<String> str = String::NewFromUtf8(isolate, "{}", NewStringType::kNormal).ToLocalChecked();
  //printf("empty: %i \n", str.IsEmpty());
  MaybeLocal<Value> json = JSON::Parse(isolate, str);
  //printf("empty: %i \n", json.IsEmpty());
  //printf("%s\n", *String::Utf8Value(json.ToLocalChecked()->ToObject(isolate)->Get(context, String::NewFromUtf8(isolate, "Id")).ToLocalChecked()->ToString(context).ToLocalChecked()));
  //objects.insert(objects.end(), Persistent<Value, CopyablePersistentTraits<Value>>(isolate, json.ToLocalChecked()->ToObject(isolate)));
  Persistent<Value, CopyablePersistentTraits<Value>> *obj = new Persistent<
      Value, CopyablePersistentTraits<Value>>(
      isolate, json.ToLocalChecked()->ToObject(isolate));
  unsigned int j = 0;
  for (unsigned int i = 0; i < objects.size(); i++) {
    if (!objects[i]) {
      objects[i] = obj;
      j = i + 1;
    }
  }
  if (j == 0) {
    j = objects.size();
    objects.insert(objects.end(), obj);
  }
  return j;
}

int Parse(std::ifstream* s, const std::string& path) {
  std::string res = "";
  std::string line;
  if (s->is_open()) {
    while (getline(*s, line)) {
      res += line + "\n";
    }
    s->close();
  } else
    perror((std::string("Unable to open file: ") + path + "\n").c_str());
  return Parse(res);
}

std::string GetString(size_t id, const std::string& key) {
  if (id >= 0 && objects.size() >= id) {
    if (objects[id]) {
      Isolate::Scope isolate_scope(isolate);
      HandleScope handle_scope(isolate);
      //Local<Context> context = Local<Context>::New(isolate, contextp);
      Local<Value> json = Local<Value>::New(isolate, *objects[id]);
      if (!json.IsEmpty()) {
        if (json->ToObject(isolate)->Has(
            String::NewFromUtf8(isolate, key.c_str()))) {
          printf(
              "return: %s\n",
              *String::Utf8Value(
                  json->ToObject(isolate)->Get(
                      String::NewFromUtf8(isolate, key.c_str()))));
          return std::string(
              *String::Utf8Value(
                  json->ToObject(isolate)->Get(
                      String::NewFromUtf8(isolate, key.c_str()))));
        } else {
          perror("JSON: no such key");
          return "error";
        }
      } else {
        perror("JSON: attempted to access an empty whatever");
        return "error";
      }
    } else {
      perror("JSON: object at x id does not exist");
      return "error";
    }
  } else {
    perror("JSON: invalid Id");
    return "error";
  }
}

void Dispose(size_t id) {
  if (id >= 0 && objects.size() >= id) {
    if (objects[id]) {
      objects[id]->Reset();
    }
    objects[id] = NULL;
  }
}

}

void CallFunction(const std::string& s) {
  Isolate::Scope isolate_scope(isolate);
  HandleScope handle_scope(isolate);

  //Context::Scope context_scope(isolate->GetCurrentContext());
  //printf("boats float because they are heavy. their weight stops them from flying into the air.\n");
  //Local<Function> f = Local<Function>::Cast(isolate->GetCurrentContext()->Global()->Get(String::NewFromUtf8(isolate, "birds", v8::NewStringType::kNormal).ToLocalChecked()));
  //Context::Scope context_scope(isolate->GetCallingContext());
  //printf("Is empty: %i\n", isolate->GetCurrentContext().IsEmpty());
  //printf("Is empty?: %i\n", contextp.IsEmpty());
  Local<Context> context = Local<Context>::New(isolate, p_context);
  Local<Value> birds = context->Global()->Get(
      context, String::NewFromUtf8(isolate, s.c_str())).ToLocalChecked();
  //printf("Is context empty?: %i\n", context.IsEmpty());
  //printf("Is birds a function?: %i\n", birds->IsFunction());
  Local<Value> res;
  Local<Value> args[1] = { String::NewFromUtf8(isolate, "arg") };
  res = Local<Function>::Cast(birds)->Call(context, context->Global(), 1, args)
      .ToLocalChecked();
  //printf("result: %s\n", *String::Utf8Value(res));
}

void Destroy() {
  //Isolate::Scope isolate_scope(isolate);
  p_kobj_node.Reset();
  p_context.Reset();
  p_gupdate.Reset();
  p_input.Reset();
  printf("current isolate: %p\n", Isolate::GetCurrent());
  isolate->Dispose();
  V8::Dispose();
  V8::ShutdownPlatform();
  //delete contextp;
  delete platform;
}

void Eval(const std::string& s) {
  Eval(s.c_str());
}

void Eval(const char* s) {
  Isolate::Scope isolate_scope(isolate);
  HandleScope handle_scope(isolate);
  Local<Context> context = Local<Context>::New(isolate, p_context);
  Context::Scope context_scope(context);
  //std::cout << s << "\n";
  Local<String> source = String::NewFromUtf8(isolate, s, NewStringType::kNormal)
      .ToLocalChecked();
  MaybeLocal<Script> script = Script::Compile(context, source);
  if (script.IsEmpty()) {
    printf("something went wrong\n");
  } else {
    MaybeLocal<Value> mayberesult = script.ToLocalChecked()->Run(context);
    if (mayberesult.IsEmpty()) {
      printf("Error!\n");
    } //else
      //Local<Value> result = mayberesult.ToLocalChecked();
  }
}

void GlobalUpdate() {
  Isolate::Scope isolate_scope(isolate);
  HandleScope handle_scope(isolate);
  Local<Context> context = Local<Context>::New(isolate, p_context);
  Context::Scope context_scope(context);
  Local<Array> a = Local<Array>::New(isolate, p_gupdate);
  for (uint16_t i = 0; i < a->Length(); i ++) {
    Local<Function>::Cast(a->Get(i))->Call(context, context->Global(), 0, NULL);
  }
}

void Setup() {

  printf("Initializing V8, version: %s\n", V8::GetVersion());

  V8::InitializeICU();
  V8::InitializeExternalStartupData(Kondion::dir);
  platform = platform::CreateDefaultPlatform();
  V8::InitializePlatform(platform);
  V8::Initialize();

  // Create a new Isolate and make it the current one.
  allocator = new ArrayBufferAllocator;
  Isolate::CreateParams create_params;
  create_params.array_buffer_allocator = allocator;
  isolate = Isolate::New(create_params);
  Isolate::Scope isolate_scope(isolate);

  // Create a stack-allocated handle scope.
  HandleScope handle_scope(isolate);

  // Make global object for the context
  Local<ObjectTemplate> global = ObjectTemplate::New(isolate);

  // This is a test
  global->Set(String::NewFromUtf8(isolate, "bunny"),
              String::NewFromUtf8(isolate, "chirp chirp"));

  // Create kdion object
  Local<ObjectTemplate> kdion = ObjectTemplate::New(isolate);
  kdion->Set(String::NewFromUtf8(isolate, "log"),
             FunctionTemplate::New(isolate, Callback_Kdion_Log));
  kdion->Set(String::NewFromUtf8(isolate, "initialize"),
             FunctionTemplate::New(isolate, Callback_Kdion_Initialize));
  kdion->Set(String::NewFromUtf8(isolate, "globalUpdate"),
             FunctionTemplate::New(isolate, Callback_Kdion_GlobalUpdate));

  // constructor functions, bird chicken test

  Local<FunctionTemplate> bird = FunctionTemplate::New(isolate, Callback_Kdion_Bird);
  bird->InstanceTemplate()->SetInternalFieldCount(1);
  bird->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "integrity"), Callback_Bird_GetIntegrity, Callback_Bird_SetIntegrity);
  kdion->Set(String::NewFromUtf8(isolate, "Bird"), bird);

  Local<FunctionTemplate> chicken = FunctionTemplate::New(isolate, Callback_Kdion_Bird);
  chicken->Inherit(bird);
  chicken->InstanceTemplate()->SetInternalFieldCount(1);

  // KObj constructors

  Local<FunctionTemplate> kobj_node = FunctionTemplate::New(isolate);
  kobj_node->InstanceTemplate()->SetInternalFieldCount(1);
  kobj_node->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "getParent"), FunctionTemplate::New(isolate, Callback_KObj_GetParent));
  kobj_node->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setParent"), FunctionTemplate::New(isolate, Callback_KObj_SetParent));
  //kobj_node->HasInstance(object)

  Local<FunctionTemplate> kobj_oriented = FunctionTemplate::New(isolate);
  kobj_oriented->InstanceTemplate()->SetInternalFieldCount(1);
  kobj_oriented->Inherit(kobj_node);
  kobj_oriented->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setPosition"), FunctionTemplate::New(isolate, Callback_Oriented_SetOffsetPosition));

  Local<FunctionTemplate> kobj_entity = FunctionTemplate::New(isolate, Callback_KObj_Entity);
  kobj_entity->InstanceTemplate()->SetInternalFieldCount(1);
  kobj_entity->Inherit(kobj_oriented);

  Local<FunctionTemplate> kobj_instance = FunctionTemplate::New(isolate, Callback_KObj_Entity);
  kobj_instance->InstanceTemplate()->SetInternalFieldCount(1);
  kobj_instance->Inherit(kobj_oriented);

  // GKO
  Local<FunctionTemplate> gko_world = FunctionTemplate::New(isolate, Callback_GKO_World);
  gko_world->InstanceTemplate()->SetInternalFieldCount(1);
  gko_world->Inherit(kobj_node);

  //bird->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "integrity"), Callback_Bird_GetIntegrity, Callback_Bird_SetIntegrity);

  // Add everything together

  kdion->Set(String::NewFromUtf8(isolate, "Bird"), bird);
  kdion->Set(String::NewFromUtf8(isolate, "Chicken"), chicken);

  global->Set(String::NewFromUtf8(isolate, "kdion"), kdion);

  global->Set(String::NewFromUtf8(isolate, "KObj_Node"), kobj_node);
  global->Set(String::NewFromUtf8(isolate, "KObj_Oriented"), kobj_oriented);
  global->Set(String::NewFromUtf8(isolate, "KObj_Entity"), kobj_entity);
  global->Set(String::NewFromUtf8(isolate, "KObj_Instance"), kobj_instance);

  global->Set(String::NewFromUtf8(isolate, "GKO_World"), gko_world);

  //global->Set(String::NewFromUtf8(isolate, "GKO_World"), gko_world);

  // Run a test script

  // Create a new context with global included
  Local<Context> context = Context::New(Isolate::GetCurrent(), NULL, global);
  Context::Scope context_scope(context);

  // Input, somehow objects can only be made after the context is created

  Local<Object> input = Object::New(isolate);
  //kdion->Set(String::NewFromUtf8(isolate, "input"), input);
  // really? (second actually, bottom one came first)
  context->Global()->Get(context, String::NewFromUtf8(isolate, "kdion")).ToLocalChecked()
      ->ToObject()->Set(String::NewFromUtf8(isolate, "input"), input);

  // World
  printf("something\n");
  Local<Value> o = gko_world->GetFunction()->CallAsConstructor(context, 0, NULL).ToLocalChecked();
  printf("something else\n");
  Local<Object> world = o->ToObject(isolate);
  printf("woot\n");
  Kondion::worldObject = static_cast<KObj::GKO_World*>(Local<External>::Cast(world->GetInternalField(0))->Value());
  // really?
  context->Global()->Get(context, String::NewFromUtf8(isolate, "kdion")).ToLocalChecked()
      ->ToObject()->Set(String::NewFromUtf8(isolate, "World"), world);

  // Create a string containing the JavaScript source code.
  Local<String> source =
      String::NewFromUtf8(
          isolate,
          "birds = function(a) {"
          "    kdion.log(\"bird says: \" + a + \" bunny says: \" + bunny + \", but do turtles say other things?\");"
          "    return  \"bird says: \" + a + \" bunny says: \" + bunny;"
          "};"
          "//kdion.World = new GKO_World();"
          "eval(\"false\");"
          "birds('yeah! ' + Math.random())",
          NewStringType::kNormal).ToLocalChecked();

  // Compile the source code.
  Local<Script> script = Script::Compile(context, source).ToLocalChecked();

  // Run the script to get the result.
  //Local<Value> result =
  script->Run(context).ToLocalChecked();

  // Convert the result to an UTF8 string and print it.
  //String::Utf8Value utf8(result);
  //printf("%s\n", *utf8);

  // Make persistent handles
  p_context = Persistent<Context,
      CopyablePersistentTraits<Context>>(isolate, context);
  p_kobj_node = Persistent<FunctionTemplate,
      CopyablePersistentTraits<FunctionTemplate>>(isolate, kobj_node);
  p_gupdate = Persistent<Array,
      CopyablePersistentTraits<Array>>(isolate, Array::New(isolate, 0));
  p_input = Persistent<Object,
        CopyablePersistentTraits<Object>>(isolate, input);

}

void Start() {
  Isolate::Scope isolate_scope(isolate);
  HandleScope handle_scope(isolate);
  Local<Context> context = Local<Context>::New(isolate, p_context);
  Context::Scope context_scope(context);

  Local<Function> init = Local<Function>::New(isolate, p_initialize);
  Local<Value> args[1] = { String::NewFromUtf8(isolate, "arg") };
  init->Call(context, context->Global(), 1, args)
      .ToLocalChecked();
  p_initialize.Reset();
}

void UpdateInput() {
  Isolate::Scope isolate_scope(isolate);
  HandleScope handle_scope(isolate);
  Local<Context> context = Local<Context>::New(isolate, p_context);
  Context::Scope context_scope(context);

  Local<Object> input = Local<Object>::New(isolate, p_input);
  for (uint16_t i = 0; i < Input::Count(); i ++) {
    if (!Input::Get(i)->alternate) {
      Local<String> s = String::NewFromUtf8(isolate, Input::Get(i)->name.c_str());
      // check if input has name registered already
      //if (input->HasOwnProperty(s)) {
        //if (input->Get(s)->ToNumber()->)
      //}
      input->Set(s, Number::New(isolate, Input::Value(i)));
    }
  }

}

}
}
