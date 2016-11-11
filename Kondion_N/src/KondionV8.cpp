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

#include "include/libplatform/libplatform.h"
#include "include/v8.h"

#include "Kondion.h"

using namespace v8;

namespace Kondion { namespace JS {

	class ArrayBufferAllocator;

	Platform* platform;
	Isolate* isolate;
	ArrayBufferAllocator* allocator;
	Persistent<Context, CopyablePersistentTraits<Context>> contextp;

	class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
	public:
		virtual void* Allocate(size_t length) {
			void* data = AllocateUninitialized(length);
			return data == NULL ? data : memset(data, 0, length);
		};
		virtual void* AllocateUninitialized(size_t length) {
			return malloc(length);
		};
		virtual void Free(void* data, size_t) {
			free(data);
		};
	};

	namespace ON {

		std::vector<Persistent<Value, CopyablePersistentTraits<Value>>*> objects;

		int Parse(std::string s) {
			Isolate::Scope isolate_scope(isolate);
			HandleScope handle_scope(isolate);
			Local<Context> context = Local<Context>::New(isolate, contextp);
			Context::Scope context_scope(context);
			printf("%s", s.c_str());
			Local<String> str = String::NewFromUtf8(isolate, s.c_str(), NewStringType::kNormal).ToLocalChecked();
			//Local<String> str = String::NewFromUtf8(isolate, "{}", NewStringType::kNormal).ToLocalChecked();
			//printf("empty: %i \n", str.IsEmpty());
			MaybeLocal<Value> json = JSON::Parse(isolate, str);
			//printf("empty: %i \n", json.IsEmpty());
			//printf("%s\n", *String::Utf8Value(json.ToLocalChecked()->ToObject(isolate)->Get(context, String::NewFromUtf8(isolate, "Id")).ToLocalChecked()->ToString(context).ToLocalChecked()));
			//objects.insert(objects.end(), Persistent<Value, CopyablePersistentTraits<Value>>(isolate, json.ToLocalChecked()->ToObject(isolate)));
			Persistent<Value, CopyablePersistentTraits<Value>> *obj = new Persistent<Value, CopyablePersistentTraits<Value>>(isolate, json.ToLocalChecked()->ToObject(isolate));
			unsigned int j = 0;
			for (unsigned int i = 0; i < objects.size(); i ++) {
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

		int Parse(std::ifstream* s, std::string path) {
			std::string res = "";
			std::string line;
			if (s->is_open()) {
				while (getline(*s, line)) {
					res += line + "\n";
				}
				s->close();
			} else perror((std::string("Unable to open file: ") + path + "\n").c_str());
			return Parse(res);
		}

		std::string GetString(int id, std::string key) {
			if (id >= 0 && objects.size() >= id) {
				if (objects[id]) {
					Isolate::Scope isolate_scope(isolate);
					HandleScope handle_scope(isolate);
					//Local<Context> context = Local<Context>::New(isolate, contextp);
					Local<Value> json = Local<Value>::New(isolate, *objects[id]);
					if (!json.IsEmpty()) {
						if (json->ToObject(isolate)->Has(String::NewFromUtf8(isolate, key.c_str()))) {
							printf("return: %s\n", *String::Utf8Value(json->ToObject(isolate)->Get(String::NewFromUtf8(isolate, key.c_str()))));
							return std::string(*String::Utf8Value(json->ToObject(isolate)->Get(String::NewFromUtf8(isolate, key.c_str()))));
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

		void Dispose(int id) {
			if (id >= 0 && objects.size() >= id) {
				if (objects[id]) {
					objects[id]->Reset();
				}
				objects[id] = NULL;
			}
		}

	}

	void Callback_Kdion_Log(const v8::FunctionCallbackInfo<v8::Value>& args) {
		if (args.Length() < 1) return;
		HandleScope handle_scope(isolate);
		printf("%s", *String::Utf8Value(args[0]));
	}


	void CallFunction(std::string s) {
		Isolate::Scope isolate_scope(isolate);
		HandleScope handle_scope(isolate);

		//Context::Scope context_scope(isolate->GetCurrentContext());
		//printf("boats float because they are heavy. their weight stops them from flying into the air.\n");
		//Local<Function> f = Local<Function>::Cast(isolate->GetCurrentContext()->Global()->Get(String::NewFromUtf8(isolate, "birds", v8::NewStringType::kNormal).ToLocalChecked()));
		//Context::Scope context_scope(isolate->GetCallingContext());
		//printf("Is empty: %i\n", isolate->GetCurrentContext().IsEmpty());
		//printf("Is empty?: %i\n", contextp.IsEmpty());
		Local<Context> context = Local<Context>::New(isolate, contextp);
		Local<Value> birds = context->Global()->Get(context, String::NewFromUtf8(isolate, s.c_str())).ToLocalChecked();
		//printf("Is context empty?: %i\n", context.IsEmpty());
		//printf("Is birds a function?: %i\n", birds->IsFunction());
		Local<Value> res;
		Local<Value> args[1] = {String::NewFromUtf8(isolate, "arg")};
		res = Local<Function>::Cast(birds)->Call(context, context->Global(), 1, args).ToLocalChecked();
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

	void Eval(std::string s) {

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

		global->Set(
			String::NewFromUtf8(isolate, "bunny"),
			String::NewFromUtf8(isolate, "chirp chirp"));

		// Create kdion object
		Local<ObjectTemplate> kdion = ObjectTemplate::New(isolate);
		kdion->Set(String::NewFromUtf8(isolate, "log"), FunctionTemplate::New(isolate, Callback_Kdion_Log));

		global->Set(String::NewFromUtf8(isolate, "kdion"), kdion);

		// Create a new context.
		Local<Context> context = Context::New(Isolate::GetCurrent(), NULL, global);

		contextp = Persistent<Context, CopyablePersistentTraits<Context>>(isolate, context);

		// Enter the context for compiling and running the hello world script.
		Context::Scope context_scope(context);

		// Create a string containing the JavaScript source code.
		Local<String> source =
			String::NewFromUtf8(isolate,
					"birds = function(a) {"
					"    kdion.log(\"bird says: \" + a + \" bunny says: \" + bunny + \", but do turtles say other things?\\n\");"
					"    return  \"bird says: \" + a + \" bunny says: \" + bunny;"
					"};"
					""
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

	void UpdateInput() {

	}

}}
