/*
 * KondionWindow.h
 *
 *  Created on: Aug 22, 2016
 *      Author: neal
 */

#ifndef KONDION_H_
#define KONDION_H_

// or Bristlecone
#define KONDION_VERSION "0.1.0 Bayberry"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <istream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

namespace Kondion {

// Base kobj classes

class KComponent;
// avoiding a circular dependency for the first time

/** Base class for all KObjs, only a node in the tree.
 *  type: 0
 */
class KObj_Node {
 public:
  std::string name;
  virtual int getType() {
    return 0;
  }
  void setParent(KObj_Node* node);
  virtual void updateA() {
  }
  virtual void updateB() {
  }
  virtual ~KObj_Node() {
  }
 protected:
  KObj_Node* parent = NULL;
};

// GKO, type 1

/** Base class for Entity and Instance, has an orientation in the world.
 *  type: 2
 */
class KObj_Oriented : public KObj_Node {
 public:
  glm::mat4x4 offset;
  glm::mat4x4 transform;
  int woot = 0;
  int farX, farY, farZ;
  int getType() {
    return 2;
  }
  virtual void parentTransform();
  //virtual ~KObj_Oriented() {};
};

/** Used to represent actual objects in the game world through Components;
 *  type: 3
 */
class KObj_Entity : public KObj_Oriented {
 public:
  std::vector<KComponent*> components;
  int getType() {
    return 3;
  }
  void render();
  void parentTransform();
};

/** Refers to an entity, but still its own object; use this so
 * that entities do not have to be copied multiple times.
 *  type: 3
 */
class KObj_Instance : public KObj_Oriented {
 public:
  int getType() {
    return 4;
  }
  void render() {
  }
};

// Component part
class KComponent {
 public:
  bool collide;
  bool renderable;
  KObj_Entity* parent;  // this caused my first circular dependency
  glm::mat4x4 offset;
  virtual void render() {

  }
  virtual ~KComponent() {

  }
 protected:
};

namespace Object {

class OKO_Camera_ : public KObj_Oriented {
 public:
  glm::vec3 center;
  glm::vec3 up;
  void prespective();
  void parentTransform();
};

}

namespace Component {

class CPN_Cube : public Kondion::KComponent {
 public:
  void render();
};

class CPN_InfinitePlane : public Kondion::KComponent {
 public:
  void render();
};
}

extern char* dir;
extern std::vector<KObj_Node *> world;

void Launch();
void GameLoop();

namespace Renderer {
extern Object::OKO_Camera_* currentCamera;
void Setup();
void Three(uint16_t width, uint16_t height);
void RenderCube(float scale);
void RenderQuad(float width, float height);
}

namespace Window {

extern GLFWwindow* w;
int Initialize();
int CreateWindow(uint16_t width, uint16_t height);
int SwitchWindow(size_t index);
bool Active();
void Update();

}

namespace Input {
const uint8_t INPUT_SYSTEM = 0, INPUT_VIRTUAL = 1, INPUT_CONTROLLER = 10;

const uint16_t MOUSE_BUTTON = 400, MOUSE_POSX = 409, MOUSE_POSY = 410,
    MOUSE_INSIDE = 411, MOUSE_SCROLLX = 412, MOUSE_SCROLLY = 413,
    CONTROLLER_AXIS = 100, CONTROLLER_BUTTON = 0;

struct Control {
  std::string name;
  bool alternate;
  uint8_t device;
  uint16_t button;
  //float x, y, px, py;
  float x, px;
  float delta() {
    return px - x;
  }
};

uint16_t AddControl(const std::string& name, uint8_t device, uint16_t button);
void MouseLock(bool a);
Control* Get(uint16_t i);
void Setup();
void Update();
uint16_t ControlIndex(const std::string& name);

void DebugPrint();
}

namespace Math {
void transform3f(const glm::mat4& a, const glm::vec3& b);

}

namespace Resources {

class KTexture {
 public:
  uint16_t width, height;
  bool isInternal, isLoaded, mipmapped;
  //unsigned short imageWidth, imageHeight;
  GLint minFilter, magFilter, textureId, wrapS, wrapT;
  std::string source;

  KTexture(GLint id, uint16_t width, uint16_t height,
           GLint miFilter, GLint maFilter, GLint awrapS, GLint awrapT,
           bool mipped);
  KTexture(std::string path, GLint miFilter, GLint maFilter, GLint awrapS,
           GLint awrapT);
  void Bind();
};

class Raw {
 public:
  std::istream* stream;
  std::filebuf* fb;
  Raw(std::string file) {
    fb = new std::filebuf;

    fb->open(file.c_str(), std::ios::in);
    stream = new std::istream(fb);
  }
  ;
  ~Raw() {
    printf("raw deleted\n");
    fb->close();
    delete stream;
    delete fb;
  }
  ;
};

void AddCarton(const std::string& path);
void Setup();
Raw* Get(const std::string& url);

extern std::vector<KTexture *> textures;

}

namespace JS {

void CallFunction(const std::string& s);
void Destroy();
void Eval(const char* s);
void Eval(const std::string& s);
void Setup();
void Start();
void UpdateInput();

namespace ON {
int Parse(const std::string& s);
int Parse(std::ifstream* s, const std::string& path);
std::string GetString(size_t id, const std::string& key);
void Dispose(size_t id);
}

}

namespace Debug {
void printMatrix(const glm::mat4& a);

}

}
#endif /* KONDION_H_ */
