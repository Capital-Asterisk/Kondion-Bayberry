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
#include "V8_Functions.h"
#include "V8_OKO_Functions.h"
#include "V8_Ent_Functions.h"

using namespace v8;

namespace Kondion {
namespace JS {

namespace ON {

std::vector<Persistent<Value, CopyablePersistentTraits<Value>>*> objects;

size_t Parse(const std::string& s) {
  Isolate::Scope isolate_scope(isolate);
  HandleScope handle_scope(isolate);
  Local<Context> context = Local<Context>::New(isolate, p_context);
  Context::Scope context_scope(context);
  //printf("%s", s.c_str());
  //printf("Parsing some JSON\n");
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
    if (objects[i] == NULL) {
      //printf("taking space: %i\n", i);
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

size_t Parse(std::ifstream* s, const std::string& path) {

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
  // TODO make this readable
  if (id >= 0 && objects.size() >= id) {
    if (objects[id]) {
      Isolate::Scope isolate_scope(isolate);
      HandleScope handle_scope(isolate);
      //Local<Context> context = Local<Context>::New(isolate, contextp);
      Local<Value> json = Local<Value>::New(isolate, *objects[id]);
      if (!json.IsEmpty()) {
        if (json->ToObject(isolate)->Has(
            String::NewFromUtf8(isolate, key.c_str()))) {
          //printf(
          //    "return: %s\n",
          //    *String::Utf8Value(
          //        json->ToObject(isolate)->Get(
          //            String::NewFromUtf8(isolate, key.c_str()))));
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

void GetStringArray(size_t id, const std::string& key, std::vector<std::string> &in) {
  // TODO: remove todo message
  // TODO: make this readable
  if (id >= 0 && objects.size() >= id) {
    if (objects[id]) {
      // Make V8 stuff work
      Isolate::Scope isolate_scope(isolate);
      HandleScope handle_scope(isolate);
      
      // Open the object that id specifies, the array with key is inside
      Local<Value> json = Local<Value>::New(isolate, *objects[id]);
      
      // Make sure the object isn't empty
      if (!json.IsEmpty()) {
        // Check if the object has the key [array]
        if (json->ToObject(isolate)->Has(
            String::NewFromUtf8(isolate, key.c_str()))) {
          
          // Get the object as an array, TODO: check if array
          Local<Array> object = Local<Array>::Cast(
              json->ToObject(isolate)->Get(
                  String::NewFromUtf8(isolate, key.c_str())));
                  
          //std::vector<std::string> ret;
          
          // Push all the elements into in
          for (uint16_t i = 0; i < object->Length(); i ++) {
            in.push_back(std::string(*String::Utf8Value(object->Get(i))));
          }
        } else {
          perror("JSON: no such key");
        }
      } else {
        perror("JSON: attempted to access an empty whatever\n");
      }
    } else {
      perror("JSON: object at x id does not exist\n");
    }
  } else {
    perror("JSON: invalid Id\n");
  }
}

void GetKeys(size_t id, std::vector<std::string> &in) {
  // Check if the Id is valid
  if (id < 0 || objects.size() < id)
    return;

  Isolate::Scope isolate_scope(isolate);
  HandleScope handle_scope(isolate);
  Local<Context> context = Local<Context>::New(isolate, p_context);

  // Get get the object as a Local<Value>
  Local<Value> objA = Local<Value>::New(isolate, *objects[id]);

  // terminate if there are errors
  if (objA.IsEmpty() || !objA->IsObject())
    return;

  // Get keys
  Local<Array> keys = objA->ToObject(isolate)->GetPropertyNames(context).ToLocalChecked();

  for (uint16_t i = 0; i < keys->Length(); i++) {
    //printf("Key: %s\n", *String::Utf8Value(keys->Get(i)));
    in.push_back(std::string(*String::Utf8Value(keys->Get(i))));
  }

}

size_t Enter(size_t id, const std::string& key) {

  // TODO make the other functions look nice like this.

  // Check if the Id is valid
  if (id < 0 || objects.size() < id)
    return -1;

  Isolate::Scope isolate_scope(isolate);
  HandleScope handle_scope(isolate);

  // Get get the object as a Local<Value>
  Local<Value> objA = Local<Value>::New(isolate, *objects[id]);

  // terminate if there are errors
  if (objA.IsEmpty() || !objA->IsObject())
    return -1;

  // terminate if the key does not exist
  if (!objA->ToObject(isolate)->Has(String::NewFromUtf8(isolate, key.c_str())))
    return -1;

  // Get object
  Local<Object> objB = Local<Object>::Cast(
      objA->ToObject(isolate)->Get(String::NewFromUtf8(isolate, key.c_str())));

  Persistent<Value, CopyablePersistentTraits<Value>> *objC = new Persistent<
        Value, CopyablePersistentTraits<Value>>(
        isolate, objB);

  size_t j = 0;
  for (size_t i = 0; i < objects.size(); i++) {
    if (objects[i] == NULL) {
      printf("taking space: %u\n", i);
      objects[i] = objC;
      return i;
    }
  }
  if (j == 0) {
    j = objects.size();
    objects.insert(objects.end(), objC);
  }
  return j;
}

void Dispose(size_t id) {
  //Isolate::Scope isolate_scope(isolate);
  //HandleScope handle_scope(isolate);
  if (id >= 0 && objects.size() >= id) {
    if (objects[id] != NULL) {
      //printf("Disposing: %i\n", id);
      objects[id]->Reset();
      //delete objects[id];
      //printf("size: %i\n", objects.size());
      //objects[id] = NULL;
    }
  }
}

} // ON namespace ends here btw

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

void DebugObjPrint(char* str) {
  Isolate::Scope isolate_scope(isolate);
  HandleScope handle_scope(isolate);
  Local<Context> context = Local<Context>::New(isolate, p_context);
  Local<Object> obj = context->Global()
      ->Get(context, String::NewFromUtf8(isolate, "kdion")).ToLocalChecked()
      ->ToObject()->Get(context, String::NewFromUtf8(isolate, "debug"))
          .ToLocalChecked()->ToObject();
  if (obj.IsEmpty())
    return;

  Local<Array> keys = obj->GetPropertyNames(context).ToLocalChecked();

  std::string printme = "";

  for (uint16_t i = 0; i < keys->Length(); i++) {
    printme += std::string(*String::Utf8Value(keys->Get(i))) + ": "
               + std::string(*String::Utf8Value(obj->Get(keys->Get(i))));
    if (i != keys->Length() - 1)
      printme += "\n";
  }
  
  Renderer::DebugText(printme);
}

void Destroy() {
  //Isolate::Scope isolate_scope(isolate);
  p_kobj_node.Reset();
  p_oko_camera.Reset();
  p_context.Reset();
  p_component.Reset();
  p_material.Reset();
  p_gupdate.Reset();
  p_input.Reset();
  //for (uint16_t i = 0; i < ON::objects.size(); i++) {
    //printf("Reset: %i\n", i);
    //ON::objects[i]->Reset();
    //ON::objects[i] = NULL;
  //}
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
  v8::TryCatch trycatch(isolate);
  v8::ScriptOrigin origin(String::NewFromUtf8(isolate, "Unknown"));
  Local<String> source = String::NewFromUtf8(isolate, s, NewStringType::kNormal)
      .ToLocalChecked();
  MaybeLocal<Script> script = Script::Compile(context, source, &origin);

  if (script.IsEmpty()) {
    v8::String::Utf8Value exception(trycatch.Exception());
    v8::Local<v8::Message> message = trycatch.Message();
    fprintf(stderr, "[JS]: something went compilingly wrong!\n");
    fprintf(stderr, "[JS]: %s\n", *exception);
    fprintf(stderr, "[JS]: Line %i\n",  message->GetLineNumber(context).FromJust());
  } else {
    MaybeLocal<Value> mayberesult = script.ToLocalChecked()->Run(context);
    if (mayberesult.IsEmpty()) {
      perror("Error!\n");
    }  //else
       //Local<Value> result = mayberesult.ToLocalChecked();
  }
}

void GlobalUpdate() {
  Isolate::Scope isolate_scope(isolate);
  HandleScope handle_scope(isolate);
  Local<Context> context = Local<Context>::New(isolate, p_context);
  Context::Scope context_scope(context);
  Local<Array> a = Local<Array>::New(isolate, p_gupdate);

  for (uint16_t i = 0; i < a->Length(); i++) {
    Local<Function>::Cast(a->Get(i))->Call(context, context->Global(), 0, NULL);
  }
}

void Setup() {

  printf("Initializing V8, version: %s\n", V8::GetVersion());

  V8::InitializeICU();
  V8::InitializeExternalStartupData(Kondion::Dir().c_str());
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
  kdion->Set(String::NewFromUtf8(isolate, "load"),
             FunctionTemplate::New(isolate, Callback_Kdion_Load));
  kdion->Set(String::NewFromUtf8(isolate, "log"),
             FunctionTemplate::New(isolate, Callback_Kdion_Log));
  kdion->Set(String::NewFromUtf8(isolate, "require"),
             FunctionTemplate::New(isolate, Callback_Kdion_Require));
  kdion->Set(String::NewFromUtf8(isolate, "initialize"),
             FunctionTemplate::New(isolate, Callback_Kdion_Initialize));
  kdion->Set(String::NewFromUtf8(isolate, "globalUpdate"),

             FunctionTemplate::New(isolate, Callback_Kdion_GlobalUpdate));
  kdion->SetAccessor(
        String::NewFromUtf8(isolate, "camera"), Callback_KObj_World_GetCamera,
        Callback_KObj_World_SetCamera);
  kdion->SetAccessor(
          String::NewFromUtf8(isolate, "delta"), Callback_Kdion_Delta,
          Callback_KObj_World_SetCamera);


  // constructor functions, bird chicken test

  Local<FunctionTemplate> bird = FunctionTemplate::New(isolate,
                                                       Callback_Kdion_Bird);
  bird->InstanceTemplate()->SetInternalFieldCount(1);
  bird->InstanceTemplate()->SetAccessor(
      String::NewFromUtf8(isolate, "integrity"), Callback_Bird_GetIntegrity,
      Callback_Bird_SetIntegrity);
  kdion->Set(String::NewFromUtf8(isolate, "Bird"), bird);

  Local<FunctionTemplate> chicken = FunctionTemplate::New(isolate,
                                                          Callback_Kdion_Bird);
  chicken->Inherit(bird);
  chicken->InstanceTemplate()->SetInternalFieldCount(1);

  // Component

  Local<FunctionTemplate> kcomponent = FunctionTemplate::New(
      isolate, Callback_Component);
  kcomponent->InstanceTemplate()->SetInternalFieldCount(1);

  kcomponent->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "setMatrix"),
      FunctionTemplate::New(isolate, Callback_Component_SetMatrix));
      
  // KMaterial
  
  Local<FunctionTemplate> kmaterial = FunctionTemplate::New(
      isolate, Callback_Material);
  kmaterial->InstanceTemplate()->SetInternalFieldCount(1);
  
  kmaterial->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "setUniform"),
      FunctionTemplate::New(isolate, Callback_Material_SetUniform));


  // *** KObj constructors

  // ** Node
  Local<FunctionTemplate> kobj_node = FunctionTemplate::New(isolate);
  kobj_node->InstanceTemplate()->SetInternalFieldCount(1);
  kobj_node->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "setName"),
      FunctionTemplate::New(isolate, Callback_KObj_SetName));
  kobj_node->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "getParent"),
      FunctionTemplate::New(isolate, Callback_KObj_GetParent));
  kobj_node->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "setParent"),
      FunctionTemplate::New(isolate, Callback_KObj_SetParent));
  //kobj_node->HasInstance(object)

  // ** Oriented
  Local<FunctionTemplate> kobj_oriented = FunctionTemplate::New(isolate);
  kobj_oriented->InstanceTemplate()->SetInternalFieldCount(1);
  kobj_oriented->Inherit(kobj_node);
  kobj_oriented->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "getPosition"),
      FunctionTemplate::New(isolate, Callback_OKO_GetOffsetPosition));
  kobj_oriented->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "setPosition"),
      FunctionTemplate::New(isolate, Callback_OKO_SetOffsetPosition));

  kobj_oriented->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "translate"),
      FunctionTemplate::New(isolate, Callback_OKO_Translate));
  kobj_oriented->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "rotate"),
      FunctionTemplate::New(isolate, Callback_OKO_Rotate));
  kobj_oriented->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "pointAt"),
      FunctionTemplate::New(isolate, Callback_OKO_PointAt));

  kobj_oriented->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "dirFd"),
      FunctionTemplate::New(isolate, Callback_OKO_DirFd));
  kobj_oriented->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "dirRt"),
      FunctionTemplate::New(isolate, Callback_OKO_DirRt));
  kobj_oriented->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "dirUp"),
      FunctionTemplate::New(isolate, Callback_OKO_DirUp));

  // ** Entity
  Local<FunctionTemplate> kobj_entity = FunctionTemplate::New(
      isolate, Callback_KObj_Entity);
  kobj_entity->InstanceTemplate()->SetInternalFieldCount(1);
  kobj_entity->Inherit(kobj_oriented);
  kobj_entity->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "addComponent"),
      FunctionTemplate::New(isolate, Callback_Entity_AddComponent));
  kobj_entity->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "physLevel"),
      FunctionTemplate::New(isolate, Callback_Entity_PhysLevel));
  kobj_entity->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "setMaterial"),
      FunctionTemplate::New(isolate, Callback_Entity_SetMaterial));
      
  // Physics stuff
  //kobj_entity->PrototypeTemplate()->Set(
  //    String::NewFromUtf8(isolate, "thrust"),
  //    FunctionTemplate::New(isolate, Callback_Kdion_Blank));
  kobj_entity->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "getVelocity"),
      FunctionTemplate::New(isolate, Callback_Entity_GetVelocity));
  kobj_entity->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "thrustN"),
      FunctionTemplate::New(isolate, Callback_Entity_ApplyForce));
  kobj_entity->PrototypeTemplate()->Set(
      String::NewFromUtf8(isolate, "accel"),
      FunctionTemplate::New(isolate, Callback_Entity_Accelerate));


  Local<FunctionTemplate> kobj_instance = FunctionTemplate::New(
      isolate, Callback_KObj_Entity);
  kobj_instance->InstanceTemplate()->SetInternalFieldCount(1);
  kobj_instance->Inherit(kobj_oriented);

  // ** GKO
  Local<FunctionTemplate> gko_world = FunctionTemplate::New(isolate,
                                                            Callback_GKO_World);
  gko_world->InstanceTemplate()->SetInternalFieldCount(1);
  gko_world->Inherit(kobj_node);

  // ** OKO

  Local<FunctionTemplate> oko_camera = FunctionTemplate::New(isolate,
                                                              Callback_OKO_Camera);
  oko_camera->InstanceTemplate()->SetInternalFieldCount(1);
  oko_camera->Inherit(kobj_oriented);
  
  // ** RKO
  Local<FunctionTemplate> rko_sky = FunctionTemplate::New(isolate,
                                                            Callback_RKO_Sky);
  rko_sky->InstanceTemplate()->SetInternalFieldCount(1);
  rko_sky->Inherit(kobj_oriented);

  //bird->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "integrity"), Callback_Bird_GetIntegrity, Callback_Bird_SetIntegrity);

  // Add everything together

  kdion->Set(String::NewFromUtf8(isolate, "Bird"), bird);
  kdion->Set(String::NewFromUtf8(isolate, "Chicken"), chicken);

  global->Set(String::NewFromUtf8(isolate, "kdion"), kdion);

  global->Set(String::NewFromUtf8(isolate, "KCompnent"), kcomponent);
  global->Set(String::NewFromUtf8(isolate, "KMaterial"), kmaterial);
  
  global->Set(String::NewFromUtf8(isolate, "KObj_Node"), kobj_node);
  global->Set(String::NewFromUtf8(isolate, "KObj_Oriented"), kobj_oriented);
  global->Set(String::NewFromUtf8(isolate, "KObj_Entity"), kobj_entity);
  global->Set(String::NewFromUtf8(isolate, "KObj_Instance"), kobj_instance);

  global->Set(String::NewFromUtf8(isolate, "GKO_World"), gko_world);
  global->Set(String::NewFromUtf8(isolate, "OKO_Camera"), oko_camera);
  global->Set(String::NewFromUtf8(isolate, "RKO_Sky"), rko_sky);

  //global->Set(String::NewFromUtf8(isolate, "GKO_World"), gko_world);

  // Run a test script

  // Create a new context with global included
  Local<Context> context = Context::New(Isolate::GetCurrent(), NULL, global);
  Context::Scope context_scope(context);

  // Input, somehow objects can only be made after the context is created

  Local<Object> input = Object::New(isolate);
  //kdion->Set(String::NewFromUtf8(isolate, "input"), input);
  // really? (second actually, bottom one came first)
  context->Global()->Get(context, String::NewFromUtf8(isolate, "kdion"))
      .ToLocalChecked()->ToObject()->Set(String::NewFromUtf8(isolate, "input"),
                                         input);

  // World
  //printf("something\n");
  Local<Value> o = gko_world->GetFunction()->CallAsConstructor(context, 0, NULL)
      .ToLocalChecked();
  //printf("something else\n");
  Local<Object> world = o->ToObject(isolate);
  //printf("woot\n");
  KObj_Node::worldObject = static_cast<KObj::GKO_World*>(Local<External>::Cast(
      world->GetInternalField(0))->Value());
  // really?
  context->Global()->Get(context, String::NewFromUtf8(isolate, "kdion"))
      .ToLocalChecked()->ToObject()->Set(String::NewFromUtf8(isolate, "World"),
                                         world);

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
  p_context = Persistent<Context, CopyablePersistentTraits<Context>>(isolate,
                                                                     context);
  p_component = Persistent<FunctionTemplate,
       CopyablePersistentTraits<FunctionTemplate>>(isolate, kcomponent);
  p_material = Persistent<FunctionTemplate,
       CopyablePersistentTraits<FunctionTemplate>>(isolate, kmaterial);
  p_kobj_node = Persistent<FunctionTemplate,
      CopyablePersistentTraits<FunctionTemplate>>(isolate, kobj_node);
  p_gupdate = Persistent<Array, CopyablePersistentTraits<Array>>(
      isolate, Array::New(isolate, 0));
  p_input = Persistent<Object, CopyablePersistentTraits<Object>>(isolate,
                                                                 input);
  p_oko_camera = Persistent<FunctionTemplate,
        CopyablePersistentTraits<FunctionTemplate>>(isolate, oko_camera);
}

void Start() {
  Isolate::Scope isolate_scope(isolate);
  HandleScope handle_scope(isolate);
  Local<Context> context = Local<Context>::New(isolate, p_context);
  Context::Scope context_scope(context);

  Local<Function> init = Local<Function>::New(isolate, p_initialize);
  Local<Value> args[1] = { String::NewFromUtf8(isolate, "arg") };
  init->Call(context, context->Global(), 1, args).ToLocalChecked();
  p_initialize.Reset();
}

void UpdateInput() {

  // Apply inputs to the 'inputs' js object.

  // fancy V8 things
  Isolate::Scope isolate_scope(isolate);
  HandleScope handle_scope(isolate);
  Local<Context> context = Local<Context>::New(isolate, p_context);
  Context::Scope context_scope(context);

  //Get a input object handle through the persistent
  Local<Object> input = Local<Object>::New(isolate, p_input);

  // Put all control values into the object
  for (uint16_t i = 0; i < Input::Count(); i++) {
    if (!Input::Get(i)->alternate) {
      Local<String> s = String::NewFromUtf8(isolate,
                                            Input::Get(i)->name.c_str());
      // check if input has name registered already
      //if (input->HasOwnProperty(s)) {
      //if (input->Get(s)->ToNumber()->)
      //}
      input->Set(s, Number::New(isolate, Input::Value(i)));
    }
  }

  // same thing, but with virtual joysticks
  for (uint16_t i = 0; i < Input::VirtualJoystick::vsticks.size(); i++) {
    // TODO: make this more efficient
    Local<String> sx = String::NewFromUtf8(isolate,
        (Input::VirtualJoystick::vsticks[i]->name + "_X").c_str());
    Local<String> sy = String::NewFromUtf8(isolate,
        (Input::VirtualJoystick::vsticks[i]->name + "_Y").c_str());
    input->Set(sx, Number::New(isolate, Input::VirtualJoystick::vsticks[i]->x));
    input->Set(sy, Number::New(isolate, Input::VirtualJoystick::vsticks[i]->y));
  }

}

std::string* ParseShader(std::string* in, Resources::KShader& mat) {
  // again, fancy V8 things
  Isolate::Scope isolate_scope(isolate);
  HandleScope handle_scope(isolate);
  Local<Context> context = Local<Context>::New(isolate, p_context);
  Context::Scope context_scope(context);

  MaybeLocal<Value> parserA = context->Global()->Get(context, String::NewFromUtf8(isolate, "kdion"))
        .ToLocalChecked()->ToObject()->Get(context, String::NewFromUtf8(isolate, "materialParser"));

  if (parserA.IsEmpty()) {
    printf("[JS/TWM]: Material parser function not found.\n");
    return NULL;
  }

  Local<Value> parserB = parserA.ToLocalChecked();
  if (!parserB->IsFunction()) {
     printf("[JS/TWM]: Invalid material parser, must be a function.\n");
     return NULL;
  }

  Local<Value> value;
  // TODO: use external string resource if not lazy
  Local<Value> args[1] = { String::NewFromUtf8(isolate, in->c_str()) };
  value = Local<Function>::Cast(parserB)->Call(context, context->Global(), 1, args)
        .ToLocalChecked();

  if (value->IsString()) {
    // Error!
    return new std::string(*String::Utf8Value(value));
  }

  if (value->IsObject()) {

    Local<Array> uniforms = Local<Array>::Cast(value->ToObject()->Get(
        String::NewFromUtf8(isolate, "uniforms")));

    Local<Value> final = value->ToObject()->Get(
        String::NewFromUtf8(isolate, "result"));
    mat.uniformCount = uniforms->Length();
    mat.textureCount = Local<Array>::Cast(value->ToObject()->Get(
        String::NewFromUtf8(isolate, "utexture")))->Length();

    mat.uniforms = new std::string[mat.uniformCount];
    mat.uniformTypes = new uint8_t[mat.uniformCount];

    // Put uniform names into the shader object
    // Strings are in this format:
    // type,name

    int comma;
    std::string sudoaptgetinstall;

    for (uint16_t i = 0; i < mat.uniformCount; i ++) {
      sudoaptgetinstall = std::string(*String::Utf8Value(uniforms->Get(i)));
      comma = sudoaptgetinstall.find(",");
      mat.uniforms[i] = sudoaptgetinstall.substr(comma + 1);

      // String switch by hashing the first 2 characters of the type
      // TODO: vectors and stuff

      switch((uint16_t(sudoaptgetinstall[0]) << 8)
              | uint8_t(sudoaptgetinstall[1])) {
        case 26990: // in for int
          mat.uniformTypes[i] = 0;
          //printf("INT\n");
          break;

        case 30057: // ui for uint
          mat.uniformTypes[i] = 1;
          //printf("UINT\n");
          break;

        case 26220: // fl for float
          mat.uniformTypes[i] = 10;
          //printf("FLOAT\n");
          break;

        case 25711: // do for double
          mat.uniformTypes[i] = 11;
          printf("[TWM] DOUBLE NOT YET IMPLEMENTED");
          break;

        case 29797: // te for texture
          mat.uniformTypes[i] = 30;
          //printf("TEXTURE\n");
          break;
      }
      
      //printf("McE: (%u) %s\n", (uint16_t(sudoaptgetinstall[comma + 1]) << 8)
      //                          | uint8_t(sudoaptgetinstall[comma + 1]),
      //       mat.uniforms[i].c_str());
    }
    
    if (final->IsString()) {
      // Tada!
      return new std::string(*String::Utf8Value(final));
    }
  }


  //printf("Returned: %s\n", *String::Utf8Value(res));

  return NULL;
}

}
}
