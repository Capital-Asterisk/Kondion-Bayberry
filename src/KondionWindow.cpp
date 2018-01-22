/*
 * Window.cpp
 *
 *  Created on: Aug 22, 2016
 *      Author: neal
 */

#include <iostream>
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

// debug stuff
bool terminalMode = false;

void ErrorCallback(int error, const char* desc) {
  printf("[GLFW] ERROR: %s\n", desc);
}

void KeyCallback(GLFWwindow* linuxisbetter, int k, int sc, int a, int m) {
  if (a == GLFW_PRESS || a == GLFW_REPEAT) {
    //cout << "Typed: " << char(c) << "\n";
    // Toggle terminal mode if tide is pressed

    if (terminalMode) {
      switch (k) {
        case GLFW_KEY_BACKSPACE:
          if (m && GLFW_MOD_SHIFT)
            JS::Eval(std::string("kdion.debug[\">\"] = \"\";"));
          else
            JS::Eval(std::string("kdion.debug[\">\"] = kdion.debug[\">\"].slice(0,-1);"));
          break;
        case GLFW_KEY_ENTER:
          if (m && GLFW_MOD_SHIFT)
            JS::Eval(std::string("kdion.debug[\">\"] += \"\\n\";"));
          else
            // Evalception
            JS::Eval(std::string("try {kdion.debug.out = eval(kdion.debug[\">\"]);} catch (e) {kdion.debug.out = + e.name + \": \" + e.message;}"));
          break;
        case GLFW_KEY_V: // paste
          if (m && GLFW_MOD_CONTROL) {
            std::string clipboard = std::string(glfwGetClipboardString(w));
            // Add escape characters to some chars
            for (uint16_t i = 0; i < clipboard.length(); i ++) {
              if (clipboard[i] == '\n') {
                clipboard[i] = 'n';
                clipboard.insert(i, 1, '\\');
                i ++; // this seems bad
              } else if (clipboard[i] == '"' || clipboard[i] == '\\') {
                clipboard.insert(i, 1, '\\');
                i ++; // this too
              }
            }
            JS::Eval(std::string("kdion.debug[\">\"] += \"" + clipboard + '"'));
          }
          break;
      }
    }
  }
}

void CharCallback(GLFWwindow* linuxisbetter, uint32_t c) {
  if (terminalMode && (char(c) != '~')) {
    if (char(c) == '"') // run with escape character
      JS::Eval(std::string("kdion.debug[\">\"] += \"\\") + char(c) + '"');
    else
      JS::Eval(std::string("kdion.debug[\">\"] += \"") + char(c) + '"');
  }
  
  // Toggle terminal mode the cool way
  terminalMode = (char(c) == '~') ^ terminalMode;

  // which is useless considering this
  if (terminalMode)
    JS::Eval(std::string("kdion.debug.e = \""
                         "\\nYou can now type into the box below"
                         "\\n[Enter] to Evaluate"
                         "\\n[Shift + Backspace] to clear all"
                         "\\n[Shift + Enter] to Indent\\n"
                         "-----------------------------------------------------"
                         "\";"));
  else
    JS::Eval(std::string("kdion.debug.e = \"Press Shift + [`] to enter command mode\";"));
}

int CreateWindow(uint16_t width, uint16_t height) {
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  //glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
  printf("GLFW Version: %s\n", glfwGetVersionString());
  int count;
  const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
  for (int i = 0; i < count; i ++) {
    printf("[GLFW] Video Mode %i, %ix%i, %iHz\n", i,
            modes[i].width, modes[i].height, modes[i].refreshRate);
  }

  w = glfwCreateWindow(width, height, "Title goes here", NULL, NULL);
  if (!w) {
    glfwTerminate();
    return -1;
  }
  glfwSetErrorCallback(ErrorCallback);
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
  //glfwSwapInterval(0);
  glfwSwapBuffers(w);
  glfwPollEvents();
}
}
}

