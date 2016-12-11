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
std::vector<KObj_Node *> world; // move this to kobj_node as static
KObj::GKO_World* worldObject;
char* dir;

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
  parent = node;
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

  JS::Start();
  //printf("objects in world: %i\n", worldObject->children.size());
  KObj::OKO_Camera_ *a = new KObj::OKO_Camera_;
  a->offset = glm::translate(a->offset, glm::vec3(0.0f, 9.7f, 0.0f));
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

  b->setParent(worldObject);
  //player->setParent(worldObject);
  a->setParent(worldObject);
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

  Input::MouseLock(true);

  //float yaw = 0.0f;
  //float pitch = 0.0f
  while (Window::Active()) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    //Debug::printMatrix(a->offset);
    //Debug::PrintMatrix)
    //a->offset = glm::translate(a->offset, glm::vec3(0.0, 0.7, 0.0));
    //a->offset += glm::vec4(0.0f, 0.001f, 0.0f, 0.0f);
    /*if (glfwGetKey(Window::w, GLFW_KEY_W)) {
     player->offset = glm::translate(player->offset, glm::vec3(0.0f, 0.0f, -0.1f));
     }

     if (glfwGetKey(Window::w, GLFW_KEY_S)) {
     a->offset = glm::translate(a->offset, glm::vec3(0.0f, 0.0f, 0.1f));
     }
     if (glfwGetKey(Window::w, GLFW_KEY_A)) {
     a->offset = glm::rotate(a->offset, 0.09f, glm::vec3(0.0f, 1.0f, 0.0f));
     }
     if (glfwGetKey(Window::w, GLFW_KEY_D)) {
     a->offset = glm::rotate(a->offset, -0.09f, glm::vec3(0.0f, 1.0f, 0.0f));
     }*/

    //if (Input::Get(Input::ControlIndex("MOUSE_BUTTON0"))->x > 0.5f) {
    //  JS::CallFunction("birds");
    //}
    JS::GlobalUpdate();


    for (size_t i = 0; i < worldObject->children.size(); i++) {
      worldObject->children[i]->updateA();
    }

    for (size_t i = 0; i < worldObject->children.size(); i++) {
      if (worldObject->children[i]->getType() == 2 || worldObject->children[i]->getType() == 3) {
        dynamic_cast<KObj_Oriented*>(worldObject->children[i])->parentTransform();
      }
    }

    // do collisions here
    // DoCollisions

    //for (size_t i = 0; i < world.size(); i ++) {
    //	world[i]->updateB();
    //}

    Renderer::Three(800, 600);

    for (size_t i = 0; i < worldObject->children.size(); i++) {
      //printf("c: %s\n", worldObject->children[i]->name.c_str());
      glPushMatrix();
      if (worldObject->children[i]->getType() == 3) {
        dynamic_cast<KObj_Entity*>(worldObject->children[i])->render();
      } else if (worldObject->children[i]->getType() == 4) {
        dynamic_cast<KObj_Instance*>(worldObject->children[i])->render();
      }
      glPopMatrix();
    }

    //a.parentTransform();

    glLoadIdentity();

    //f += 0.01f;
    //Kondion::Resources::textures[0]->Bind();
    //glTranslatef(0.0f, 0.0f, -6.0f + f);
    //glRotatef(f * 80.0f, 0.0f, 1.0f, 0.0f);
    //Renderer::RenderCube(1.0f);
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
    Window::Update();

  }
  JS::Destroy();

}

}

int main(int argc, const char* argv[]) {
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
