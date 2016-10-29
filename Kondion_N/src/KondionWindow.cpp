/*
 * Window.cpp
 *
 *  Created on: Aug 22, 2016
 *      Author: neal
 */

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ctype.h>

#include "Kondion.h"

using namespace std;

//namespace Kondion::Window {
namespace Kondion { namespace Window {

	GLFWwindow* w;

	int Initialize() {
		if (!glfwInit())
			return -1;

		return 0;
	}

	void KeyCallback(GLFWwindow* linuxisbetter, int k, int sc, int a, int m) {
		if (a == GLFW_PRESS) {

			// interpert key typed
			//cout << "Key press: " << char(k) << "(" << k << ")" << "\n";

		}
	}

	void CharCallback(GLFWwindow* linuxisbetter, unsigned int c) {
			cout << "Typed: " << char(c) << "\n";
	}

	int CreateWindow(int width, int height) {
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		w = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
		if (!w) {
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(w);
		glfwSetKeyCallback(w, KeyCallback);
		glfwSetCharCallback(w, CharCallback);
		if (glewInit() != GLEW_OK)
			std::cout << "rip\n";
		return 0;
	}

	int SwitchWindow(int index) {
		return 0;
	}

	bool Active() {
		return !glfwWindowShouldClose(w);
	}

	void Update() {

		glfwSwapBuffers(w);
		glfwPollEvents();
	}
}}

