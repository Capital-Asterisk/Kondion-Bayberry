/*
 * Window.cpp
 *
 *  Created on: Aug 22, 2016
 *      Author: neal
 */

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

//namespace Kondion::Window {
namespace Kondion { namespace Window {

	GLFWwindow* w;

	int Initialize() {
		if (!glfwInit())
			return -1;

		return 0;
	}

	int CreateWindow(int width, int height) {

		w = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
		if (!w) {
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(w);
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

