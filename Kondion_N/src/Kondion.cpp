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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
		//std::cout << "hey";
		transform = glm::mat4x4(); // identity
		//parent->wurla = 0;
		//transform[23] = 0;
		//std::cout << "pork " << parent << "\n";
		if (parent) {
			if (parent->getType() == 2)
				transform *= (dynamic_cast<KObj_Oriented*>(parent))->transform;

			//if (!transferScale) {
				//actTransform.normalize3x3();
			//}
		}
		transform *= offset;
		//actTransform.mul(transform);
	}

	void Launch() {
		cout << "Hello World\n";
		glm::vec4 f(0.0f, 1.4f, 0.0f, 1.0f);
		glm::mat4 m(1.0);

		Debug::printMatrix(m);

		//cml::matrix_rotate_about_local_x(m, 2.0f);
		//cml::matrix_rotate

		//m.operator =(glm::rotate(m, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
		m.operator =(glm::translate(m, glm::vec3(0.1f, 0.2f, 0.3f)));
		Debug::printMatrix(m);

		//cml::matrix_translation(m, 0.0f, 2.0f, 0.0f);
		//Debug::printMatrix(m);
		//cml::matrix_rotate_about_local_x(m, 2.0f);
		//f = cml::transform_vector_4D(m, f);

		//cml::transform

		//cout << f[1] << "\n";

		//Debug::printMatrix(m);

		Kondion::Window::Initialize();
		Kondion::Window::CreateWindow(800, 600);

		Kondion::Resources::Setup();
		Kondion::GameLoop();

		Kondion::world.insert(Kondion::world.end(), new Kondion::KObj_Node());
		//Kondion::world.insert(Kondion::world.end(), new Kondion::KObj_Entity());

		//Kondion::world[0]->wurla ++;
		//Kondion::world[1]->wurla = 40;

		//cout << Kondion::world[0]->wurla << " " << Kondion::world[1]->wurla << endl;

	}

	void GameLoop() {

		Kondion::Renderer::Setup();

		// Most of this is from NeHe, only temporary
		glEnable(GL_TEXTURE_2D);
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
		gluPerspective(45.0f,(GLfloat)800/(GLfloat)600,0.1f,100.0f);

		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();									//

		glClearColor(0.5f, 0.0f, 0.0f, 1.0f);

		OKO_Camera_ a;

		float f = 0.1f;
		while (Kondion::Window::Active()) {

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			Renderer::Three(&a, 800, 600);

			if (glfwGetKey(Kondion::Window::w, GLFW_KEY_W)) {
				a.offset = glm::translate(a.offset, glm::vec3(0.0f, 0.0f, -0.06f));
			}

			if (glfwGetKey(Kondion::Window::w, GLFW_KEY_S)) {
				a.offset = glm::translate(a.offset, glm::vec3(0.0f, 0.0f, 0.06f));
			}
			if (glfwGetKey(Kondion::Window::w, GLFW_KEY_A)) {
				a.offset = glm::rotate(a.offset, 0.03f, glm::vec3(0.0f, 1.0f, 0.0f));
			}
			if (glfwGetKey(Kondion::Window::w, GLFW_KEY_D)) {
				a.offset = glm::rotate(a.offset, -0.03f, glm::vec3(0.0f, 1.0f, 0.0f));
			}

			//for (size_t i = 0; i < world.size(); i ++) {
			//	world[i]->updateA();
			//}

			//for (size_t i = 0; i < world.size(); i ++) {
			//	if (world[i]->getType() == 2) {
			//		dynamic_cast<KObj_Oriented*>(world[i])->parentTransform();
			//	}
			//}

			// do collisions here
			// DoCollisions

			//for (size_t i = 0; i < world.size(); i ++) {
			//	world[i]->updateB();
			//}

			//for (size_t i = 0; i < world.size(); i ++) {
			//	if (world[i]->getType() == 3) {
			//		dynamic_cast<KObj_Entity*>(world[i])->render();
			//	} else if (world[i]->getType() == 4) {
			//		dynamic_cast<KObj_Instance*>(world[i])->render();
			//	}

			//}


			a.parentTransform();

			glLoadIdentity();
			f += 0.01f;
			Kondion::Resources::textures[0]->Bind();
			glTranslatef(0.0f, 0.0f, -6.0f + f);
			glRotatef(f * 80.0f, 0.0f, 1.0f, 0.0f);
			Renderer::RenderCube(1.0f);
			/*glBegin(GL_QUADS);
				// Front Face
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
				// Back Face
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
				// Top Face
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
				// Bottom Face
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
				// Right face
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
				// Left Face
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
			glEnd();*/
			Kondion::Window::Update();
		}
	}



}

class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
 public:
  virtual void* Allocate(size_t length) {
    void* data = AllocateUninitialized(length);
    return data == NULL ? data : memset(data, 0, length);
  }
  virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
  virtual void Free(void* data, size_t) { free(data); }
};

int main(int argc, char* argv[]) {
	// This part was taken out of google examples and glfw examples
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

	  Kondion::Launch();


	/*GLFWwindow* w;

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
