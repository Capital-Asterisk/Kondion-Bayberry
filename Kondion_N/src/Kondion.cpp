//============================================================================
// Name        : Kondion.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <chrono>
// Temporary
#include <unistd.h>

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
uint64_t startTime;
int64_t timeCurrent;
double delta;
}

std::vector<KObj_Node *> KObj_Node::all;
std::vector<KMaterial *> Kondion::KMaterial::materials;
KObj::GKO_World* KObj_Node::worldObject;

const char* KObj_Node::myClass = "error_node";
const char* KObj_Oriented::myClass = "error_oriented";
const char* KObj_Renderable::myClass = "error_renderable";
const char* KObj_Entity::myClass = "entity";
const char* KObj_Instance::myClass = "instance";
const char* KObj::GKO_World::myClass = "world";
const char* KObj::OKO_Camera_::myClass = "camera";
const char* KObj::OKO_Force::myClass = "force";

const char* KComponent::myClass = "how did this get here?";
const char* Component::CPN_Cube::myClass = "cube";
const char* Component::CPN_InfinitePlane::myClass = "infplane";

KObj_Node::KObj_Node() {
  jsObject = NULL;
  parent = NULL;
  depth = 0;
  myIndex = 0;
  treeSize = 0;
  drawLayer = 1;
  name = "NotSmoothBlockModel";
  //printf("hey there\n");
  // TODO make this not overflow
  all.push_back(this);
  allIndex = all.size() - 1;
}

void KObj_Node::setParent(KObj_Node* node) {
  // TODO loop parenting, changing parent
  if (node == this) {
    // Prevent self parenting
    perror("An object will never be parented to itself.\n");
    return;
  }
  if (parent != NULL) {
    // TODO change parent
    //parent->children[myIndex] = NULL;
  }
  //printf("my type: %i, ", this->getType());
  parent = node;
  bool entity = this->getType() == 3 || this->getType() == 4;

  if (entity) {
    // Add to renderer if renderable
    KObj_Renderable* a = (dynamic_cast<KObj_Renderable*>(this));
    Renderer::Consider(a);
  }
  for (uint16_t i = 0; i < parent->children.size(); i++) {
    if (parent->children[i] == NULL) {
      parent->children[i] = this;
      myIndex = i;
      printf("parent has been set, the other way.\n");
      return;
    }
  }
  //printf("parent has been set.\n");
  myIndex = parent->children.size();
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
  topObject = top->allIndex;
  //printf("Top: %s\n", top->name);
  // pointer comparison, (instanceof world)
  if (top->getClass() == KObj::GKO_World::myClass) {
    // it's a world, add it to the list of the entire tree
    // mightbedone TODO: do something that puts the node into the right place

    KObj::GKO_World* world = static_cast<KObj::GKO_World*>(top);

    //printf("Index: %i\n", index);
    world->world.insert(world->world.begin() + index - 1, allIndex);

    if (entity) {
      // I am terrain
      if (static_cast<KObj_Entity*>(this)->physics == 0)
        world->terrain.push_back(allIndex);
    } else if (getClass() == KObj::OKO_Force::myClass) {
      // i am a force, add to forces
      world->forces.push_back(allIndex);
    }
    //KObj::GKO_World::worldObject->world.push_back(allIndex);

  }
  //Debug::printWorld();
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

  rotVelocity = glm::normalize(rotVelocity);
  orientation = orientation * glm::toMat4(glm::mix(
          glm::quat(glm::vec3(0.0, 0.0, 0.0)),
          rotVelocity, float(delta) * 32));
  if (physics != 0) {
    mass = 1.0f;
    radialMass = mass / 12.0f; // moment of inertia for a 1x1 cube
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

double Delta() {
  return delta;
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

uint64_t TimeMs() {
  return timeCurrent - startTime;
}

void GameLoop() {

  Renderer::RenderPass::New(Renderer::RenderPass::DEFAULT, 1, 0, 0, true);

  JS::Start();

  // TODO: make a nice constructor, or not
  // gravity!
  KObj::OKO_Force* gravity = new KObj::OKO_Force;
  gravity->type = 0;
  gravity->direction = 1;
  gravity->shape = 0;
  gravity->falloff = 0;
  gravity->strength = 9.80665223;
  gravity->size = 0;
  // rotate to point downwards
  gravity->orientation = glm::rotate(
    gravity->orientation, glm::pi<float>() / 2,
    glm::vec3(1.0f, 0.0f, 0.0f));
  gravity->name = "Gravity";
  gravity->setParent(KObj_Node::worldObject);

  //player->setParent(worldObject);
  //a->setParent(KObj_Node::worldObject);
  //w.insert(world.end(), b);
  //world.insert(world.end(), player);
  //world.insert(world.end(), a);

  Input::AddControl("MOUSE_X", Input::INPUT_SYSTEM, Input::MOUSE_POSX);
  Input::AddControl("MOUSE_Y", Input::INPUT_SYSTEM, Input::MOUSE_POSY);
  Input::AddControl("MOUSE_BUTTON0", Input::INPUT_SYSTEM, Input::MOUSE_BUTTON);
  //Input::AddControl("D_R", Input::INPUT_CONTROLLER, Input::CONTROLLER_AXIS + 0);
  //Input::AddControl("D_U", Input::INPUT_CONTROLLER, Input::CONTROLLER_AXIS + 1);
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
  Input::AddControl("DEBUGC", Input::INPUT_SYSTEM, ' ');

  f->add(Input::ControlIndex("D_U"), 0, 1.0f);
  f->add(Input::ControlIndex("D_L"), 64, 1.0f);
  f->add(Input::ControlIndex("D_D"), 128, 1.0f);
  f->add(Input::ControlIndex("D_R"), 192, 1.0f);
  
  Resources::KShader::shaders[0]->Load(true);

  Input::MouseLock(true);

  glfwSetTime(15.0);

  double lastTime = glfwGetTime();
  double currentTime = 0;

  //float yaw = 0.0f;
  //float pitch = 0.0f
  while (Window::Active()) {

    // egg timing
    timeCurrent = std::chrono::duration_cast < std::chrono::milliseconds
        > (std::chrono::system_clock::now().time_since_epoch()).count();

    // delta timing
    currentTime = glfwGetTime();
    delta = currentTime - lastTime;
    lastTime = currentTime;
    
    //delta *= 0.2f;

    KObj_Node::worldObject->sceneTime += delta;

    if (currentTime > 16.0) {
      glfwSetTime(currentTime - 16.0);
      lastTime -= 16.0;
      Debug::printWorld();
      
    }

    //printf("Time: (%u)(%f)\n", TimeMs() / 1000, KObj_Node::worldObject->sceneTime);

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

    if (Input::Get(Input::ControlIndex("DEBUGC"))->x) {
      // Temp jump
      static_cast<KObj_Entity*>(KObj_Node::all[3])->velocity.y += 0.9;
    }

    // do collisions here
    // DoCollisions

    //for (size_t i = 0; i < world.size(); i ++) {
    //	world[i]->updateB();
    //}

    PhysicsUpdate();

    //printf("directions: %f %f\n", f->x, f->y);

    // temporary compositor
    Renderer::Composite();
    //Renderer::Three(800, 600);

    glLoadIdentity();
    Window::Update();

    Resources::Update();

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR) {
      printf("An OpenGL error happened: %#08x\n", err);
    }

    //usleep(100 * 1000);

  }
  JS::Destroy();

}

}

int main(int argc, const char* argv[]) {

  // this is an experiment
  //uint32_t a = 2147483648;
  //uint32_t b = 2;
  //uint32_t c = a * b;
  //printf("Unsigned 2147483648 * 2 = %i\n", c);
  // aparrently 2 non-zero numbers can multiply to zero

  //new Kondion::Resources::KMaterial();
  Kondion::startTime = std::chrono::duration_cast < std::chrono::milliseconds
      > (std::chrono::system_clock::now().time_since_epoch()).count();

  {
    // Get the directory
    char buf[256];
    Kondion::dir = strcat(getcwd(buf, sizeof(buf)), "/");

    // Print some information
    printf("Kondion %s | Compiled %s %s\nDirectory: %s\n", KONDION_VERSION,
        __DATE__, __TIME__, Kondion::dir);

    // Setup V8
    Kondion::JS::Setup();

    // make space for the default texture.
    Kondion::Resources::KTexture::textures.push_back(NULL);
    
    // Add default material
    Kondion::Resources::KShader::New("defmat", "kdefault:shaders/defmat");

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
