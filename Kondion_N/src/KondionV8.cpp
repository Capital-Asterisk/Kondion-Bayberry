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
  Local<Context> context = Local<Context>::New(isolate, contextp);
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
  Local<Context> context = Local<Context>::New(isolate, contextp);
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
  contextp.Reset();
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
  Local<Context> context = Local<Context>::New(isolate, contextp);
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
    } else
      Local<Value> result = mayberesult.ToLocalChecked();
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
  printf("current isolate: %p\n", Isolate::GetCurrent());

  Isolate::Scope isolate_scope(isolate);

  printf("current isolate: %p\n", Isolate::GetCurrent());

  // Create a stack-allocated handle scope.
  HandleScope handle_scope(isolate);

  // Make global object for the context
  Local<ObjectTemplate> global = ObjectTemplate::New(isolate);

  global->Set(String::NewFromUtf8(isolate, "bunny"),
              String::NewFromUtf8(isolate, "chirp chirp"));

  // Create kdion object
  Local<ObjectTemplate> kdion = ObjectTemplate::New(isolate);
  kdion->Set(String::NewFromUtf8(isolate, "log"),
             FunctionTemplate::New(isolate, Callback_Kdion_Log));
  kdion->Set(String::NewFromUtf8(isolate, "initialize"),
             FunctionTemplate::New(isolate, Callback_Kdion_Initialize));

  // Object constructors

  Local<FunctionTemplate> bird = FunctionTemplate::New(isolate, Callback_Kdion_Bird);
  bird->InstanceTemplate()->SetInternalFieldCount(1);
  bird->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "integrity"), Callback_Bird_GetIntegrity, Callback_Bird_SetIntegrity);
  kdion->Set(String::NewFromUtf8(isolate, "Bird"), bird);

  global->Set(String::NewFromUtf8(isolate, "kdion"), kdion);

  // Create a new context.
  Local<Context> context = Context::New(Isolate::GetCurrent(), NULL, global);

  contextp = Persistent<Context, CopyablePersistentTraits<Context>>(isolate,
                                                                    context);

  // Enter the context for compiling and running the hello world script.
  Context::Scope context_scope(context);

  // Create a string containing the JavaScript source code.
  Local<String> source =
      String::NewFromUtf8(
          isolate,
          "birds = function(a) {"
          "    kdion.log(\"bird says: \" + a + \" bunny says: \" + bunny + \", but do turtles say other things?\");"
          "    return  \"bird says: \" + a + \" bunny says: \" + bunny;"
          "};"
          "eval(\"false\");"
          "birds('yeah! ' + Math.random())",
          NewStringType::kNormal).ToLocalChecked();

  // Compile the source code.
  Local<Script> script = Script::Compile(context, source).ToLocalChecked();

  // Run the script to get the result.
  Local<Value> result = script->Run(context).ToLocalChecked();

  // Convert the result to an UTF8 string and print it.
  String::Utf8Value utf8(result);
  printf("%s\n", *utf8);

  //objects.Size();
}

void Start() {
  Isolate::Scope isolate_scope(isolate);
  HandleScope handle_scope(isolate);
  Local<Context> context = Local<Context>::New(isolate, contextp);
  Context::Scope context_scope(context);

  Local<Function> init = Local<Function>::New(isolate, initialize);
  Local<Value> args[1] = { String::NewFromUtf8(isolate, "arg") };
  Local<Value> res = init->Call(context, context->Global(), 1, args)
      .ToLocalChecked();
  initialize.Reset();
}

void UpdateInput() {

}

}
}
