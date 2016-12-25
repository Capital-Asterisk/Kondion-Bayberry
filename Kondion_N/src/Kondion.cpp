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
#include <glm/gtc/type_ptr.hpp>

#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "Kondion.h"

namespace Kondion {

// I really don't know how to avoid these
char* dir;

std::vector<KObj_Node *> KObj_Node::all;
KObj::GKO_World* KObj_Node::worldObject;

std::vector<Renderer::RenderPass*> Renderer::RenderPass::passes;

KObj_Node::KObj_Node() {
  jsObject = NULL;
  myIndex = 0;
  drawLayer = 1;
  name = "NotSmoothBlockModel";
  printf("hey there\n");
  all.push_back(this);
}

KObj_Node* KObj_Node::getParent() {
  return parent;
}

void KObj_Node::setParent(KObj_Node* node) {
  if (node == this) {
    perror("An object will never be parented to itself.\n");
    return;
  }
  if (parent != NULL) {
    parent->children[myIndex] = NULL;
  }
  printf("my type: %i, ", this->getType());
  parent = node;
  if (this->getType() == 3 || this->getType() == 4) {
    KObj_Renderable* a = (dynamic_cast<KObj_Renderable*>(this));
    Renderer::Consider(a);
  }
  for (uint16_t i = 0; i < parent->children.size(); i ++) {
    if (parent->children[i] == NULL) {
      parent->children[i] = this;
      printf("parent has been set, the other way.\n");
      return;
    }
  }
  printf("parent has been set.\n");
  parent->children.push_back(this);
}

void KObj_Oriented::parentTransform() {
  //std::cout << "hey";
  transform = glm::mat4x4();  // identity
  //parent->wurla = 0;
  //transform[23] = 0;
  //std::cout << "pork " << parent << "\n";
  if (parent) {
    if (parent->getType() >= 2)
      transform *= (dynamic_cast<KObj_Oriented*>(parent))->transform;

    //if (!transferScale) {
    //actTransform.normalize3x3();
    //}
  }
  transform *= offset;

  //actTransform.mul(transform);
}

void KObj_Entity::parentTransform() {
  KObj_Oriented::parentTransform();
  for (unsigned short i; i < components.size(); i++) {
    components[i]->parent = this;
  }
}

void KObj_Entity::render() {
  glMultMatrixf(glm::value_ptr(transform));
  for (unsigned short i; i < components.size(); i++) {
    if (components[i]->renderable) {
      components[i]->render();
    }

  }
}

void Launch() {
  printf("Hello World\n");

  Window::Initialize();
  Window::CreateWindow(800, 600);

  //worldObject = new KObj::GKO_World;

  Renderer::Setup();
  Input::Setup();
  Resources::Setup();

  Kondion::GameLoop();

}

void GameLoop() {

  new Renderer::RenderPass(Renderer::RenderPass::DEFAULT, 1, 0, 0, true);

  JS::Start();
  //printf("objects in world: %i\n", worldObject->children.size());
  KObj::OKO_Camera_ *a = new KObj::OKO_Camera_;
  a->offset = glm::translate(a->offset, glm::vec3(0.0f, 0.7f, 0.0f));
  a->offset = glm::rotate(a->offset, 0.3f, glm::vec3(1.0f, 0.0f, 0.0f));
  KObj_Entity *b = new KObj_Entity;
  b->components.insert(b->components.end(), new Component::CPN_InfinitePlane);
  b->components[0]->offset = glm::rotate(b->components[0]->offset, 3.14159f / 2,
                                         glm::vec3(1, 0, 0));
  b->offset = glm::translate(b->offset, glm::vec3(0.0f, -0.0f, 0.0f));
  //printf("type: %i\n", a->getType());
  //KObj_Entity *player = new KObj_Entity;
  //player->components.insert(player->components.end(), new Component::CPN_Cube);
  //player->offset = glm::translate(player->offset, glm::vec3(0.0f, 0.25f, 0.0f));
  //player->offset = glm::rotate(player->offset, 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
  //a->setParent(player);
  Renderer::currentCamera = a;

  b->setParent(KObj_Node::worldObject);
  //player->setParent(worldObject);
  a->setParent(KObj_Node::worldObject);
  //w.insert(world.end(), b);
  //world.insert(world.end(), player);
  //world.insert(world.end(), a);

  Input::AddControl("MOUSE_X", Input::INPUT_SYSTEM, Input::MOUSE_POSX);
  Input::AddControl("MOUSE_Y", Input::INPUT_SYSTEM, Input::MOUSE_POSY);
  Input::AddControl("MOUSE_BUTTON0", Input::INPUT_SYSTEM, Input::MOUSE_BUTTON);
  //Input::AddControl("MOVE_X", Input::INPUT_CONTROLLER, Input::CONTROLLER_AXIS + 0);
  //Input::AddControl("MOVE_Y", Input::INPUT_CONTROLLER, Input::CONTROLLER_AXIS + 1);
  Input::AddControl("MOVE_X", Input::INPUT_SYSTEM, 'A');
  Input::AddControl("MOVE_Y", Input::INPUT_SYSTEM, '2');
  Input::AddControl("MOVE_Y", Input::INPUT_SYSTEM, 'W');
  Input::AddControl("DEBUGA", Input::INPUT_SYSTEM, 'K');
  Input::AddControl("DEBUGB", Input::INPUT_SYSTEM, 'L');
  Input::MouseLock(true);

  //float yaw = 0.0f;
  //float pitch = 0.0f
  while (Window::Active()) {

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Input::Update();
    JS::UpdateInput();
    //Input::DebugPrint();

    //player->offset = glm::translate(
    //    player->offset,
    //    glm::vec3(0.0f, 0.0f,
    //              Input::Get(Input::ControlIndex("MOVE_Y"))->x * -0.1f));
    //player->offset = glm::rotate(
    //    player->offset,
    //    (Input::Get(Input::ControlIndex("MOUSE_X"))->delta()) * 0.001f,
    //    glm::vec3(0.0f, 1.0f, -0.000000f));

    glm::vec3 trans = glm::vec3(a->offset[3]);
    a->offset = glm::mat4x4();
    a->offset = glm::translate(a->offset, trans);
    a->offset = glm::rotate(
            a->offset, Input::Get(Input::ControlIndex("MOUSE_X"))->x * -0.001f,
            glm::vec3(0.0f, 1.0f, 0.0f));
    a->offset = glm::rotate(
        a->offset, Input::Get(Input::ControlIndex("MOUSE_Y"))->x * -0.001f,
        glm::vec3(1.0f, -0.0f, -0.0f));

    //glm::value_ptr(a->offset)[13] = 0.7f;
    a->offset = glm::translate(a->offset, glm::vec3(0, 0, -Input::Value(Input::ControlIndex("MOVE_Y")) * 0.4));
    //b->offset = glm::rotate(b->offset, 0.01f, glm::vec3(1.0f, 0.0f, 0.0f));
    JS::GlobalUpdate();


    for (size_t i = 0; i < KObj_Node::worldObject->children.size(); i++) {
      KObj_Node::worldObject->children[i]->updateA();
    }

    for (size_t i = 0; i < KObj_Node::worldObject->children.size(); i++) {
      if (KObj_Node::worldObject->children[i]->getType() == 2 || KObj_Node::worldObject->children[i]->getType() == 3) {
        dynamic_cast<KObj_Oriented*>(KObj_Node::worldObject->children[i])->parentTransform();
      }
    }

    // do collisions here
    // DoCollisions

    //for (size_t i = 0; i < world.size(); i ++) {
    //	world[i]->updateB();
    //}

    // temporary compositor
    Renderer::Composite();
    //Renderer::Three(800, 600);

    glLoadIdentity();
    Window::Update();

  }
  JS::Destroy();

}

}

int main(int argc, const char* argv[]) {
  uint32_t a = 2147483648;
  uint32_t b = 2;
  uint32_t c = a * b;
  printf("Unsigned 2147483648 * 2 = %i\n", c);
  {
    char buf[256];
    Kondion::dir = strcat(getcwd(buf, sizeof(buf)), "/");
    printf("Kondion %s | %s %s\nDirectory:%s\n", KONDION_VERSION, __DATE__,
           __TIME__, Kondion::dir);
    Kondion::JS::Setup();

    unsigned char mode = 1;  // 0: none, 1: carton
    for (int i = 1; i < argc; i++) {
      switch (mode) {
        case 1:
          Kondion::Resources::AddCarton(std::string(argv[i]));
          break;

      }
      //Xprintf("arg: %i: %s\n", i, argv[i]);
    }

  }
  // object oriented at its best
  Kondion::Launch();
  printf("Exit successful!");
  return 0;
}
