//============================================================================
// Name        : Kondion.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//====================== gbb vggttttggggggggggggg======================================================

#include <chrono>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "Kondion.h"

namespace Kondion {

namespace {
char* dir;
uint64_t timeNow;
int64_t timeCurrent;
}

std::vector<KObj_Node *> KObj_Node::all;
KObj::GKO_World* KObj_Node::worldObject;

const std::string KObj_Node::myClass = "error_node";
const std::string KObj_Oriented::myClass = "error_oriented";
const std::string KObj_Renderable::myClass = "error_renderable";
const std::string KObj_Entity::myClass = "entity";
const std::string KObj_Instance::myClass = "instance";
const std::string KObj::GKO_World::myClass = "world";
const std::string KObj::OKO_Camera_::myClass = "camera";
const std::string KObj::OKO_Force::myClass = "force";

std::vector<Renderer::RenderPass*> Renderer::RenderPass::passes;

KObj_Node::KObj_Node() {
  jsObject = NULL;
  parent = NULL;
  depth = 0;
  myIndex = 0;
  treeSize = 0;
  drawLayer = 1;
  name = "NotSmoothBlockModel";
  printf("hey there\n");
  // TODO make this not overflow
  all.push_back(this);
  allIndex = all.size() - 1;
}

KObj_Node* KObj_Node::getParent() {
  return parent;
}

void KObj_Node::setParent(KObj_Node* node) {
  // TODO loop parenting, changing parent
  if (node == this) {
    // Prevent self parenting
    perror("An object will never be parented to itself.\n");
    return;
  }
  if (parent != NULL) {
    // TODO
    //parent->children[myIndex] = NULL;
  }
  printf("my type: %i, ", this->getType());
  parent = node;
  if (this->getType() == 3 || this->getType() == 4) {
    KObj_Renderable* a = (dynamic_cast<KObj_Renderable*>(this));
    Renderer::Consider(a);
  }
  for (uint16_t i = 0; i < parent->children.size(); i++) {
    if (parent->children[i] == NULL) {
      parent->children[i] = this;
      printf("parent has been set, the other way.\n");
      return;
    }
  }
  printf("parent has been set.\n");
  parent->children.push_back(this);
  depth = parent->depth + 1;
  // world related stuff
  // Go through the chain if parents, check for parent looping, and treeSize ++
  KObj_Node* top = parent;
  uint16_t index = 0;
  for (uint8_t i = 0; i < depth; i ++) {
    index += top->myIndex + 1;
    top->treeSize ++;
    if (top->depth != 0) {
      top = top->parent;
    }
  }
  printf("Top: %s\n", top->name);
  // TODO: instanceof world
  if (top == KObj::GKO_World::worldObject) {
    // it's a world, add it to the list of the entire tree
    // TODO: do something that puts the node into the right place
    printf("Index: %i\n", index);
    KObj::GKO_World::worldObject->world.insert(
        KObj::GKO_World::worldObject->world.begin() + index - 1,
        allIndex);
    //KObj::GKO_World::worldObject->world.push_back(allIndex);

  }
  Debug::printWorld();
}

void KObj_Oriented::parentTransform() {
  //std::cout << "hey";
  transform = glm::mat4x4();  // identity
  //parent->wurla = 0;
  //transform[23] = 0;
  //std::cout << "pork " << parent << "\n";
  if (parent) {
    if (parent->getType() >= 2)
      transform *= (static_cast<KObj_Oriented*>(parent))->transform;

    //if (!transferScale) {
    //actTransform.normalize3x3();
    //}
  }
  transform *= orientation;

  //actTransform.mul(transform);
}

void KObj_Entity::updateA() {
  //rotVelocity = glm::quat(glm::vec3(0.0, 0.01, 0.0));
  velocity.y += 0.000004f;
  orientation = orientation * glm::toMat4(rotVelocity);
  if (physics != 0) {
    orientation[3][0] += velocity.x;
    orientation[3][1] += velocity.y;
    orientation[3][2] += velocity.z;
  }
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

std::string Dir() {
  return std::string(dir);
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
  //KObj::OKO_Camera_ *a = new KObj::OKO_Camera_;
  //a->offset = glm::translate(a->offset, glm::vec3(0.0f, 0.7f, 0.0f));
  //a->offset = glm::rotate(a->offset, 0.3f, glm::vec3(1.0f, 0.0f, 0.0f));
  KObj_Entity *b = new KObj_Entity;
  b->components.insert(b->components.end(), new Component::CPN_InfinitePlane);
  b->components[0]->offset = glm::rotate(b->components[0]->offset, 3.14159f / 2,
                                         glm::vec3(1, 0, 0));
  b->orientation = glm::translate(b->orientation, glm::vec3(0.0f, -0.0f, 0.0f));
  b->physics = 0;
  b->name = "Ground";
  //printf("type: %i\n", a->getType());
  //KObj_Entity *player = new KObj_Entity;
  //player->components.insert(player->components.end(), new Component::CPN_Cube);
  //player->offset = glm::translate(player->offset, glm::vec3(0.0f, 0.25f, 0.0f));
  //player->offset = glm::rotate(player->offset, 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
  //a->setParent(player);
  //Renderer::currentCamera = a;

  b->setParent(KObj_Node::worldObject);

  // TODO: make a nice constructor, or not
  // gravity!
  KObj::OKO_Force* gravity = new KObj::OKO_Force;
  gravity->type = 0;
  gravity->direction = 1;
  gravity->shape = 0;
  gravity->falloff = 0;
  gravity->strength = 1;
  gravity->size = 0;
  // rotate to point downwards
  gravity->orientation = glm::rotate(
    gravity->orientation, -glm::pi<float>() / 2,
    glm::vec3(1.0f, 0.0f, 0.0f));
  gravity->name = "Gravity";


  //player->setParent(worldObject);
  //a->setParent(KObj_Node::worldObject);
  //w.insert(world.end(), b);
  //world.insert(world.end(), player);
  //world.insert(world.end(), a);

  Input::AddControl("MOUSE_X", Input::INPUT_SYSTEM, Input::MOUSE_POSX);
  Input::AddControl("MOUSE_Y", Input::INPUT_SYSTEM, Input::MOUSE_POSY);
  Input::AddControl("MOUSE_BUTTON0", Input::INPUT_SYSTEM, Input::MOUSE_BUTTON);
  //Input::AddControl("D_R", Input::INPUT_CONTROLLER, Input::CONTROLLER_AXIS + 6);
  //Input::AddControl("D_U", Input::INPUT_CONTROLLER, Input::CONTROLLER_AXIS + 7);
  Input::VirtualJoystick* f = new Input::VirtualJoystick(true, "MOVE");
  Input::AddControl("D_U", Input::INPUT_SYSTEM, 'W');
  Input::AddControl("D_L", Input::INPUT_SYSTEM, 'A');
  Input::AddControl("D_D", Input::INPUT_SYSTEM, 'S');
  Input::AddControl("D_R", Input::INPUT_SYSTEM, 'D');

  //Input::AddControl("MOVE_X", Input::INPUT_SYSTEM, 'A');
  //Input::AddControl("MOVE_Y", Input::INPUT_SYSTEM, '2');
  //Input::AddControl("MOVE_Y", Input::INPUT_SYSTEM, 'W');


  Input::AddControl("DEBUGA", Input::INPUT_SYSTEM, 'K');
  Input::AddControl("DEBUGB", Input::INPUT_SYSTEM, 'L');

  f->add(Input::ControlIndex("D_U"), 0, 1.0f);
  f->add(Input::ControlIndex("D_L"), 64, 1.0f);
  f->add(Input::ControlIndex("D_D"), 128, 1.0f);
  f->add(Input::ControlIndex("D_R"), 192, 1.0f);

  Input::MouseLock(true);

  double startTime = glfwGetTime();
  double lastTime = startTime;
  double currentTime = 0;
  double delta;

  glfwSetTime(15.0);

  //float yaw = 0.0f;
  //float pitch = 0.0f
  while (Window::Active()) {

    // egg timing
    timeCurrent = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    // delta timing
    currentTime = glfwGetTime();
    delta = currentTime - lastTime;
    lastTime = currentTime;

    if (currentTime > 16.0) {
      glfwSetTime(currentTime - 16.0);
      lastTime -= 16.0;

      Debug::printWorld();
    }

    //printf("Time: %f\n", currentTime);

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

    //glm::vec3 trans = glm::vec3(a->offset[3]);
    //a->offset = glm::mat4x4();
    //a->offset = glm::translate(a->offset, trans);
    //a->offset = glm::rotate(
    //    a->offset, Input::Get(Input::ControlIndex("MOUSE_X"))->x * -0.001f,
    //    glm::vec3(0.0f, 1.0f, 0.0f));
    //a->offset = glm::rotate(
    //    a->offset, Input::Get(Input::ControlIndex("MOUSE_Y"))->x * -0.001f,
    //    glm::vec3(1.0f, -0.0f, -0.0f));

    //glm::value_ptr(a->offset)[13] = 0.7f;
    //a->offset = glm::translate(
    //    a->offset,
    //    glm::vec3(0, 0, -Input::Value(Input::ControlIndex("MOVE_Y")) * 0.4));
    //b->offset = glm::rotate(b->offset, 0.01f, glm::vec3(1.0f, 0.0f, 0.0f));
    JS::GlobalUpdate();

    for (size_t i = 0; i < KObj_Node::worldObject->children.size(); i++) {
      KObj_Node::worldObject->children[i]->updateA();
    }

    // All objects in the world tree are looped through
    for (size_t i = 0; i < KObj_Node::worldObject->world.size(); i++) {
      if (KObj_Node::all[KObj_Node::worldObject->world[i]]->getType() == 2
          || KObj_Node::all[KObj_Node::worldObject->world[i]]->getType() == 3) {
        static_cast<KObj_Oriented*>(KObj_Node::all[KObj_Node::worldObject->world[i]])
            ->parentTransform();
      }
    }

    // do collisions here
    // DoCollisions

    //for (size_t i = 0; i < world.size(); i ++) {
    //	world[i]->updateB();
    //}

    //printf("directions: %f %f\n", f->x, f->y);

    // temporary compositor
    Renderer::Composite();
    //Renderer::Three(800, 600);

    glLoadIdentity();
    Window::Update();

    Resources::Update();

  }
  JS::Destroy();

}

}

int main(int argc, const char* argv[]) {

  // this is an experiment
  uint32_t a = 2147483648;
  uint32_t b = 2;
  uint32_t c = a * b;
  printf("Unsigned 2147483648 * 2 = %i\n", c);
  // aparrently 2 non-zero numbers can multiply to zero

  {
    // Get the directory
    char buf[256];
    Kondion::dir = strcat(getcwd(buf, sizeof(buf)), "/");

    // Print some information
    printf("Kondion %s | %s %s\nDirectory:%s\n", KONDION_VERSION, __DATE__,
    __TIME__,
           Kondion::dir);

    // Setup V8
    Kondion::JS::Setup();

    // make space for the default texture.
    Kondion::Resources::KTexture::textures.push_back(NULL);

    // Load cartons, goal: loading screen asap
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
