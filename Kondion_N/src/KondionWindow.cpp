/*
 * Window.cpp
 *
 *  Created on: Aug 22, 2016
 *      Author: neal
 */

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
			cout << "Key press: " << char(k) << "(" << k << ")" << "\n";
		}
	}

	int CreateWindow(int width, int height) {
		w = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
		if (!w) {
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(w);
		glfwSetKeyCallback(w, KeyCallback);
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

