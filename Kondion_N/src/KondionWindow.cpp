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
namespace Kondion {
namespace Window {

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

void CharCallback(GLFWwindow* linuxisbetter, uint32_t c) {
  //cout << "Typed: " << char(c) << "\n";
}

int CreateWindow(uint16_t width, uint16_t height) {
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  w = glfwCreateWindow(width, height, "Title goes here", NULL, NULL);
  if (!w) {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(w);
  glfwSetKeyCallback(w, KeyCallback);
  glfwSetCharCallback(w, CharCallback);
  if (glewInit() != GLEW_OK)
    perror("GLFW Initialization error\n");
  return 0;
}

int SwitchWindow(uint8_t index) {
  return 0;
}

uint16_t GetWidth(uint8_t i) {
  int width, height;
  glfwGetWindowSize(w, &width, &height);
  return width;
}

uint16_t GetHeight(uint8_t i) {
  int width, height;
  glfwGetWindowSize(w, &width, &height);
  return height;
}

bool Active() {
  return !glfwWindowShouldClose(w);
}

void Update() {

  glfwSwapBuffers(w);
  glfwPollEvents();
}
}
}

