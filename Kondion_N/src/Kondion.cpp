//============================================================================
// Name        : Kondion.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cml/cml.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "include/libplatform/libplatform.h"
#include "include/v8.h"

#include "Kondion.h"

using namespace v8;
using namespace std;

namespace Kondion {

	std::vector<KObj_Node *> world;

	void KObj_Oriented::parentTransform() {
		parent->wurla = 0;
		//transform[23] = 0;
		if (parentOrient) {

		}
	}

	void Launch() {
		cout << "Hello World\n";
		cml::vector4f f(0.0f, 5.0f, 0.0f, 1.0f);
		cml::matrix44f m;

		cml::matrix_rotate_about_local_z(m, 1.0f);

		cml::transform_vector_4D(m, f);
		//cml::transform

		cout << f[1] << "\n";

		Kondion::Window::Initialize();
		Kondion::Window::CreateWindow(800, 600);
		Kondion::GameLoop();

		Kondion::world.insert(Kondion::world.end(), new Kondion::KObj_Node());
		//Kondion::world.insert(Kondion::world.end(), new Kondion::KObj_Entity());

		//Kondion::world[0]->wurla ++;
		//Kondion::world[1]->wurla = 40;

		//cout << Kondion::world[0]->wurla << " " << Kondion::world[1]->wurla << endl;

	}

	void GameLoop() {

		// Most of this is from NeHe, only temporary
		glShadeModel(GL_SMOOTH);
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
		glClearDepth(1.0f);									// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	//

		glViewport(0,0,800,600);						// Reset The Current Viewport

		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix

		// Calculate The Aspect Ratio Of The Window
		gluPerspective(45.0f,(GLfloat)640/(GLfloat)480,0.1f,100.0f);

		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();									//

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		float f = 0.1f;
		while (Kondion::Window::Active()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();
			f += 0.001f;
			glTranslatef(-1.5f, 0,-6.0f + f);	//
			glBegin(GL_QUADS);                      // Draw A Quad
			glVertex3f(-1.0f, 1.0f, 0.0f);              // Top Left
			        glVertex3f( 1.0f, 1.0f, 0.0f);              // Top Right
			        glVertex3f( 1.0f,-1.0f, 0.0f);              // Bottom Right
			        glVertex3f(-1.0f,-1.0f, 0.0f);              // Bottom Left
			    glEnd();                            // Done Drawing The Quad
			Kondion::Window::Update();
		}
	}



}

/*class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
 public:
  virtual void* Allocate(size_t length) {
    void* data = AllocateUninitialized(length);
    return data == NULL ? data : memset(data, 0, length);
  }
  virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
  virtual void Free(void* data, size_t) { free(data); }
};*/

int main(int argc, char* argv[]) {

	Kondion::Launch();
	/* This part was taken out of google examples and glfw examples
	cout << "Hello world from the binary" << endl;

	V8::InitializeICU();
	  V8::InitializeExternalStartupData(argv[0]);
	  Platform* platform = platform::CreateDefaultPlatform();
	  V8::InitializePlatform(platform);
	  V8::Initialize();

	  // Create a new Isolate and make it the current one.
	  ArrayBufferAllocator allocator;
	  Isolate::CreateParams create_params;
	  create_params.array_buffer_allocator = &allocator;
	  Isolate* isolate = Isolate::New(create_params);
	  {
	    Isolate::Scope isolate_scope(isolate);

	    // Create a stack-allocated handle scope.
	    HandleScope handle_scope(isolate);

	    // Create a new context.
	    Local<Context> context = Context::New(isolate);

	    // Enter the context for compiling and running the hello world script.
	    Context::Scope context_scope(context);

	    // Create a string containing the JavaScript source code.
	    Local<String> source =
	        String::NewFromUtf8(isolate, "'Hello' + ', World From V8'",
	                            NewStringType::kNormal).ToLocalChecked();

	    // Compile the source code.
	    Local<Script> script = Script::Compile(context, source).ToLocalChecked();

	    // Run the script to get the result.
	    Local<Value> result = script->Run(context).ToLocalChecked();

	    // Convert the result to an UTF8 string and print it.
	    String::Utf8Value utf8(result);
	    printf("%s\n", *utf8);
	  }


	GLFWwindow* w;

	if (!glfwInit())
	return -1;

	w = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!w) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(w);

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	//

	glViewport(0,0,640,480);						// Reset The Current Viewport

		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix

		// Calculate The Aspect Ratio Of The Window
		gluPerspective(45.0f,(GLfloat)640/(GLfloat)480,0.1f,100.0f);

		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();									//

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	while (!glfwWindowShouldClose(w)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glTranslatef(-1.5f,0.0f,-6.0f);	//
		 glBegin(GL_QUADS);                      // Draw A Quad
		        glVertex3f(-1.0f, 1.0f, 0.0f);              // Top Left
		        glVertex3f( 1.0f, 1.0f, 0.0f);              // Top Right
		        glVertex3f( 1.0f,-1.0f, 0.0f);              // Bottom Right
		        glVertex3f(-1.0f,-1.0f, 0.0f);              // Bottom Left
		    glEnd();                            // Done Drawing The Quad
		glfwSwapBuffers(w);
		glfwPollEvents();
	}

	 isolate->Dispose();
	  V8::Dispose();
	  V8::ShutdownPlatform();
	  delete platform;
	glfwTerminate();*/
	return 0;
}
