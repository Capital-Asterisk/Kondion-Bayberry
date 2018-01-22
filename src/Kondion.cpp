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
//uint64_t timeNow;
uint64_t startTime;
int64_t timeCurrent;
double delta;
double fps;
}

std::vector<KObj_Node *> KObj_Node::all;
KObj::GKO_World* KObj_Node::worldObject;

const char* KObj_Node::myClass = "Hex Editor, oh yeah!";
const char* KObj_Oriented::myClass = "error_oriented";
const char* KObj_Renderable::myClass = "error_renderable";
const char* KObj_Entity::myClass = "entity";
const char* KObj_Instance::myClass = "instance";
const char* KObj::GKO_World::myClass = "world";
const char* KObj::OKO_Camera_::myClass = "camera";
const char* KObj::OKO_Force::myClass = "force";
const char* KObj::RKO_Sky::myClass = "sky";

const char* KComponent::myClass = "how did this get here?";
const char* Component::CPN_Cube::myClass = "cube";
const char* Component::CPN_InfinitePlane::myClass = "infplane";
const char* Component::CPN_Sphere::myClass = "sphere";
const char* Component::CPN_Mesh::myClass = "mesh";

double Delta() {
  return delta;
}

std::string Dir() {
  return std::string(dir);
}

double Fps() {
  return fps;
}

void Launch() {
  printf("Hello World\n");

  Window::Initialize();
  Window::CreateWindow(1280, 720);

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
  //KObj_Entity* moist = static_cast<KObj_Entity*>(KObj_Node::all[KObj::GKO_World::worldObject->world[2]]);
  //Component::CPN_Mesh* meshy = new Component::CPN_Mesh;
  //printf("MESHYMCMESHMESHMESHMEASDASDASDSHSDFHSDFGDFGDFSG: %p %i\n", meshy, meshy->renderable);
  //moist->components.push_back(meshy);

  glfwSetTime(15.0);

  double lastTime = glfwGetTime();
  double currentTime = 0;
  fps = 10;

  while (Window::Active()) {

    // egg timing
    timeCurrent = std::chrono::duration_cast < std::chrono::milliseconds
        > (std::chrono::system_clock::now().time_since_epoch()).count();
    //timeCurrent = glfwGetTime();
    
    // delta timing
    currentTime = glfwGetTime();
    delta = currentTime - lastTime;
    lastTime = currentTime;

    fps += ((1.0 / delta) - fps) / 20;

    //delta *= 0.5f;
    delta = 1.0 / 60.0;

    KObj_Node::worldObject->sceneTime += delta;

    if (currentTime > 16.0) {
      glfwSetTime(currentTime - 16.0);
      lastTime -= 16.0;
      Debug::printWorld();
      
    }


    Input::Update();
    JS::UpdateInput();

    JS::GlobalUpdate();

    for (size_t i = 0; i < KObj_Node::worldObject->children.size(); i++) {
      KObj_Node::worldObject->children[i]->updateA();
    }

    // All objects in the world tree are looped through
    for (size_t i = 0; i < KObj_Node::worldObject->world.size(); i++) {
      if (KObj_Node::all[KObj_Node::worldObject->world[i]]->getType() == 2
          || KObj_Node::all[KObj_Node::worldObject->world[i]]->getType() == 3
          || KObj_Node::all[KObj_Node::worldObject->world[i]]->getType() == 4) {
        static_cast<KObj_Oriented*>(KObj_Node::all[KObj_Node::worldObject->world[i]])
            ->parentTransform();
      }
    }

    PhysicsUpdate();

    //printf("directions: %f %f\n", f->x, f->y);

    // temporary compositor
    Renderer::Composite();

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

  // TODO: get a better way of getting ms time soon
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

    // make space for the default texture
    // TODO: make own vector classes
    Kondion::Resources::KTexture::textures.push_back(NULL);
    Kondion::Resources::KTexture::textures.push_back(NULL);
    Kondion::Resources::KMesh::meshes.push_back(NULL);
    
    // Add default material
    Kondion::Resources::KShader::New("defmat", "kdefault:shaders/defmat.twm");

    // Load cartons, goal: loading screen asap
    uint8_t mode = 1;  // 0: none, 1: carton
    for (int i = 1; i < argc; i++) {
      switch (mode) {
        case 1:
          Kondion::Resources::AddCarton(std::string(argv[i]));
          break;

      }
    }

  }
  // object oriented at its best
  Kondion::Launch();
  printf("Exit successful!");
  return 0;
}
