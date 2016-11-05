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

	void CallFunction(std::string s) {
		Isolate::Scope isolate_scope(isolate);
		HandleScope handle_scope(isolate);

		//Context::Scope context_scope(isolate->GetCurrentContext());
		printf("boats float because they are heavy. their weight stops them from flying into the air.\n");
		//Local<Function> f = Local<Function>::Cast(isolate->GetCurrentContext()->Global()->Get(String::NewFromUtf8(isolate, "birds", v8::NewStringType::kNormal).ToLocalChecked()));
		//Context::Scope context_scope(isolate->GetCallingContext());
		//printf("Is empty: %i\n", isolate->GetCurrentContext().IsEmpty());
		printf("Is empty: %i\n", contextp.IsEmpty());
		printf("Is function: %i\n", Local<Context>::New(isolate, contextp)->Global()->IsFunction());
		;
	}

	void Destroy() {
		//Isolate::Scope isolate_scope(isolate);
		printf("current isolate: %p\n", Isolate::GetCurrent());
		isolate->Dispose();
		V8::Dispose();
		V8::ShutdownPlatform();
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
			String::NewFromUtf8(isolate, "bunny", v8::NewStringType::kNormal).ToLocalChecked(),
			String::NewFromUtf8(isolate, "chirp chirp", v8::NewStringType::kNormal).ToLocalChecked());

		// Create a new context.
		Local<Context> context = Context::New(Isolate::GetCurrent(), NULL, global);

		contextp = Persistent<Context, CopyablePersistentTraits<Context>>(isolate, context);

		// Enter the context for compiling and running the hello world script.
		Context::Scope context_scope(context);

		// Create a string containing the JavaScript source code.
		Local<String> source =
			String::NewFromUtf8(isolate,
					"var birds = function(a) {"
					"   return  \"bird says: \" + a + \" bunny says: \" + bunny;"
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



	}

	void UpdateInput() {

	}

}}
