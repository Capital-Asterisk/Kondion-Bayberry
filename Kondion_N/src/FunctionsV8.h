/*
 * FunctionsV8.cpp
 *
 *  Created on: Nov 11, 2016
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
	Persistent<Function, CopyablePersistentTraits<Function>> initialize;

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

	void Callback_Kdion_Bird(const v8::FunctionCallbackInfo<v8::Value>& args) {
		//if (args.Length() < 1) return;
		HandleScope handle_scope(isolate);
		printf("Bird factory was called!\n");
		if (args.IsConstructCall()) {
			printf("A new bird has been created!\n");
			args.This()->Set(
					String::NewFromUtf8(isolate, "crushed"),
					Boolean::New(isolate, false));
			args.GetReturnValue().Set(args.This());
		}
	}


	void Callback_Kdion_Log(const v8::FunctionCallbackInfo<v8::Value>& args) {
		if (args.Length() < 1) return;
		HandleScope handle_scope(isolate);
		printf("%s\n", *String::Utf8Value(args[0]));
	}

	void Callback_Kdion_Initialize(const v8::FunctionCallbackInfo<v8::Value>& args) {
		if (args.Length() < 1) return;
		HandleScope handle_scope(isolate);
		Local<Value> arg0 = args[0];
		if (arg0->IsFunction()) {
			initialize.Reset();
			initialize = Persistent<Function, CopyablePersistentTraits<Function>>(isolate, Local<Function>::Cast(arg0));
		}
	}



}}
