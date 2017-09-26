// License goes here

#ifndef KONDION_H_
#define KONDION_H_

// or Bristlecone
#define KONDION_VERSION "0.1.0 Bayberry"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <istream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

// TODO: eventually replace a bunch of uint16_t with 32_t because object count

namespace Kondion {

class KObj_Node;
class KObj_Oriented;
class KObj_Renderable;
class KObj_Entity;
class KObj_Instance;

class KComponent;
class KMaterial;

struct KCurve;

namespace Resources {
class KShader;
class KTexture;
}

namespace Physics {
class CollisionInfo;
}

namespace KObj {
class GKO_World;
}

// Made for mathmatical curves, and possible animations. Suppose to generate
// common wave functions like sine, square, saw, noise, etc... This would also
// have smooth interpolation between data points, such as linear and bezier.
struct KCurve {
  uint8_t type;
  uint16_t size;
  float* points;
  KCurve(uint8_t t, uint16_t s, float* p) {
    type = t;
    size = s;
    points = p;
  }
  ;

  ~KCurve() {
    delete points;
  }
};

// Base class for all KObjs, only a node in the tree. It's only function is to
// be a node that can have more nodes in it, and also keep track of it's parent
//
// KObj_Node is sad and lonely, as it could never physically interact with any
// of it's friends.
class KObj_Node {
 public:
  // All of the KObj_Nodes in existance, including the ones that aren't in world.
  static std::vector<KObj_Node *> all;
  // The current world object, has no parent
  static KObj::GKO_World* worldObject;
  // use for identification
  static const char* myClass;
  
  // Time since scene started, affected by timescale;
  double sceneTime;
  // Name to identify with
  std::string name;
  // All the children
  std::vector<KObj_Node*> children;
  // How deep the object is in the tree. World is 0, object in world is 1.
  uint8_t depth;
  // Index out of all of the KObj_Nodes in existance
  uint16_t allIndex;
  // My index out of all of siblings
  uint16_t myIndex;
  // Index of the object up the parent chain, usually the world
  uint16_t topObject;
  // How large the tree is, this as root. (all of children's children...)
  uint16_t treeSize;
  // 32 bits of layers, use bitwise
  uint32_t drawLayer;
  // The JS object that is bound
  void* jsObject;
  virtual int getType() {
    return 0;
  }
  void setParent(KObj_Node* node);
  KObj_Node* getParent() {
    return parent;
  }
  virtual void updateA() {
  }
  virtual void updateB() {
  }
  virtual const char* getClass() {
    return myClass;
  }
  KObj_Node();
  virtual ~KObj_Node() {
  }
 protected:
  KObj_Node* parent = NULL;
};

// This type of node has a place in the physical world. Not your world where
// you exist in, but the virtual Kondion world. Oriented nodes can be parented
// to another oriented node, and it would inherit it's transformation. (follow
// it's parent around). This object is not solid, and has no mesh of any sort,
// more of a single point in space, or like an empty in Blender 3D.
// 
// Orienteds are not chinese, or asian, bug are less lonely as KObj_Nodes
class KObj_Oriented : public KObj_Node {
 public:
  static const char* myClass;
  glm::mat4x4 transform;
  glm::mat4x4 orientation;
  int woot = 0;
  int farX, farY, farZ;
  const char* getClass() {
    return myClass;
  }
  int getType() {
    return 2;
  }
  virtual void parentTransform();
  //virtual ~KObj_Oriented() {};
};

// Just why does this exist, would there ever be a renderable object that isn't
// an entity? Renderables are just made to be rendered. They are eatable by the
// renderer, most likely OpenGL.
class KObj_Renderable : public KObj_Oriented {
 public:
  static const char* myClass;
  uint16_t material = 0;
  bool complex = false;

  int getType() {
    return 3;
  }
  const char* getClass() {
    return myClass;
  }
  virtual void render()=0;
};

// Used to represent actual objects in the game world through Components.
// Entities are physical, have physics, mass, and fancy stuff. Without
// components, entities are just blank points in space that react to forces.
// Components are added to make the entity solid.
class KObj_Entity : public KObj_Renderable {
 public:

  static const char* myClass;

  //glm::mat4x4 next;
  glm::vec3 velocity;
  glm::quat rotVelocity;

  std::vector<KComponent*> components;

  // 0: static
  // 1: something else
  uint8_t physics;

  // physics stuff
  glm::vec3 acceleration;  // total calculated acceleration
  uint16_t radius;  // radius of influence (farthest component)
  float mass;
  float radialMass; // moment of inertia
  double accel;

  KObj_Entity() {
    this->components.clear();
    this->physics = 1;
    this->mass = 1.0f;
    this->radialMass = 1.0f;
  }

  const char* getClass() {
    return myClass;
  }
  int getType() {
    return 4;
  }
  
  void updateA();
  void parentTransform();
  
  virtual void render();
};

// This thing should actually inherit Entity. If there's a lot of the same
// entity, then the engine would get slow. Instances are just duplicates of an
// entity they're linked too, and are suppose to be much faster than multiple
// copies of the same entity, as this solution would use data from the entity.
// Example: Bullets
class KObj_Instance : public KObj_Renderable {
 public:
  static const char* myClass;
  const char* getClass() {
    return myClass;
  }
  int getType() {
    return 5;
  }
  virtual void render() {
  }
};

// Components are slaves to the entities, they come in many different shapes
// and sizes. Cubes, infinite planes, balls, custom 3D objects, and more!
// They can be rendered, or used as colliders.
class KComponent {
 public:
  static const char* myClass;

  bool collide;
  bool renderable;

  KObj_Entity* parent;  // this caused my first circular dependency
  void* jsObject;

  glm::mat4x4 offset; 

  // Physical properties

  virtual const char* getClass() {
    return myClass;
  }
  virtual void render() {

  }
  virtual void testCollision(KComponent& comp, Physics::CollisionInfo& ci) {

  }
  virtual ~KComponent() {
    offset = glm::mat4x4(1);
    collide = true;
    renderable = true;
    parent = NULL;
    jsObject = NULL;
  }
 protected:
};

// Materials are linked to a shader. They contain information to feed the
// shader. The shader must be loaded for it to work, as shaders are resources.
// For example: material stores colour blue, and the simple colour shader draws
// blue.
class KMaterial {
 public:
  static std::vector<KMaterial*> materials;

  bool uniformSet;
  uint16_t shader;
  void** uniforms;
  
  KMaterial() {
    uniformSet = false;
    shader = 0;
    materials.push_back(this);
  }

};

namespace KObj {

// The world where everything is stored. This node is just at the very top, and
// is probably parented to itself. It holds some information about the world,
// and keeps track of objects.
class GKO_World : public KObj_Node {
 public:
  static const char* myClass;
  float timescale = 0;
  std::vector<uint16_t> world;
  std::vector<uint16_t> terrain;
  std::vector<uint16_t> forces;

  //std::vector<uint16_t> frictionA;
  //std::vector<uint16_t> frictionB;
  //std::vector<float> frictionC;

  //std::vector<uint16_t> bounceA;
  //std::vector<uint16_t> bounceB;
  //std::vector<float> bounceC;

  const char* getClass() {
    return myClass;
  }
};

// Cheese! This oriented object is suppose to be the eye into the world.
// Multiple cameras could be made and can be switched between, or used with
// different render passes.
// 
// The strange underscore is a reference to my very first 3D game engine
class OKO_Camera_ : public KObj_Oriented {
 public:
  static const char* myClass;
  glm::vec3 center;
  glm::vec3 up;
  const char* getClass() {
    return myClass;
  }
  void prespective();
  void parentTransform();
};

// Gravity, wind, ropes, springs, rockets, and mysterous attractors. All these
// should be forces, as calculations should include acceleration.
class OKO_Force : public KObj_Oriented {
 public:

  static const char* myClass;

  // 0: typical acceleration
  // 1: constant velocity
  // 2: drag
  // 3: multiply
  uint8_t type;

  // 0: towards center
  // 1: towards orientation (OKO -z)
  // 2: towards association 0
  // 3: towards trajectory (speed up)
  uint8_t direction;

  // 0: global
  // 1: sphere/point
  // 100: associations collision
  uint8_t shape;

  // 0: linear
  // 1: inverse square
  // 2: constant
  uint8_t falloff;

  float strength;
  //float multiplier;

  // probably in meters
  float size;

  std::vector<uint16_t> associations;

  const char* getClass() {
    return myClass;
  }
  void parentTransform();
};

// Adds a sky to the world, render doesn't actually do anything
class RKO_Sky : public KObj_Renderable {
 public:
  static const char* myClass;
  void render() {
  }
  const char* getClass() {
    return myClass;
  }
};

}

namespace Physics {

// Apply a force to an object, assuming mass is 1. The object's velocity and
// rotational velocity would change. Imagine hitting a cube on the corner vs
// a face center.
//
// Position determines where the force is applied, (0, 0, 0) being center of 
// mass.
//
// Vector is the direction and magnitude of the force. Would cause rotation if
// if's not pointed at the center of mass.
//void applyVelocity(KObj_Entity* ent, glm::vec3 position, glm::vec3 vector);

// Same as applyVelocity but the... nevermind just use the one above...
// wait... actually this is more important.
// Apply a force to an object, The object's velocity and rotational velocity
// would change. Imagine hitting a cube on the corner vs a face center.
//
// Position determines where the force is applied, (0, 0, 0) being center of 
// mass.
//
// Vector is the direction and magnitude of the force. Would cause rotation if
// if's not pointed at the center of mass.
void ApplyForce(KObj_Entity* ent, glm::vec3 position, glm::vec3 force);

// Stores information about what happened on a collision.
class CollisionInfo {

 public:

  // if a collision happened
  bool collided;
  // how far A has been embedded into B
  float sink;
  // Time of contact.
  double collideTime;

  // Surface normals of collision
  glm::vec3 normA;
  glm::vec3 normB;

  // Where the collisions happened
  // relative to center of mass without rotation
  glm::vec3 spotA;
  glm::vec3 spotB;

  // Impact velocity from A-B relative to world, including rotations
  glm::vec3 impact;
  //double impactVelocity;
};

}

namespace Component {

class CPN_Cube : public KComponent {
 public:
  static const char* myClass;
  const char* getClass() {
    return myClass;
  }
  void render();
  void testCollision(KComponent& comp, Kondion::Physics::CollisionInfo& ci);
};

class CPN_InfinitePlane : public KComponent {
 public:
  static const char* myClass;
  const char* getClass() {
    return myClass;
  }
  void render();
  void testCollision(KComponent& comp, Kondion::Physics::CollisionInfo& ci);
};

class CPN_Sphere : public KComponent {
 public:
  static const char* myClass;
  const char* getClass() {
    return myClass;
  }
  void render();
  void testCollision(KComponent& comp, Kondion::Physics::CollisionInfo& ci);
};
}

double Delta();
std::string Dir();
void GameLoop();
void Launch();
void PhysicsUpdate();
double TimeScene();
uint64_t TimeMs();

namespace Renderer {
extern KObj::OKO_Camera_* currentCamera;

class RenderPass {
 public:

  static const unsigned char DEFAULT = 0, FORWARD = 1, DIFFUSE = 2, DEPTH = 3,
      NORMALS = 4, LIGHT = 5, HDR = 6, GUI = 20;
  static std::vector<RenderPass*> passes;
  static void New(uint8_t type, uint32_t drawLayer, uint16_t width,
                  uint16_t height, bool autoscan);

  bool autoscan;
  bool ready = false;
  bool framebuffered = false;
  bool cameraOverride = false;
  std::vector<KObj_Renderable*> items;
  KObj::OKO_Camera_* camera;
  KObj::RKO_Sky* sky;
  uint32_t drawLayer;
  uint16_t width, height;
  uint8_t type;

  virtual void consider(KObj_Renderable* a) {
  }

  virtual void force(KObj_Renderable* a) {
  }

  virtual void generate() {
  }

  virtual void render() {
  }

  virtual void scan() {
  }

  virtual GLuint id(uint8_t a) {
    return 0;
  }

 protected:
  //GLuint* ids;
  RenderPass() {

  }
  ;

  virtual ~RenderPass() {

  }

};

GLuint CompileShader(GLenum type, const std::string& code,
                     const std::string& errorname);
void Composite();
void Consider(KObj_Renderable* a);
void Setup();
void Three(KObj::OKO_Camera_* c, uint16_t width, uint16_t height);
void Two(uint8_t window);
void DebugText(const std::string& text);
void RenderCube(float scale);
void RenderQuad(float width, float height);
}

namespace Window {

extern GLFWwindow* w;
int Initialize();
int CreateWindow(uint16_t width, uint16_t height);
int SwitchWindow(uint8_t index);
uint16_t GetWidth(uint8_t i);
uint16_t GetHeight(uint8_t i);
bool Active();
void Update();

}

namespace Input {
const uint8_t INPUT_SYSTEM = 0, INPUT_VIRTUAL = 1, INPUT_CONTROLLER = 10;

const uint16_t MOUSE_BUTTON = 400, MOUSE_POSX = 409, MOUSE_POSY = 410,
    MOUSE_INSIDE = 411, MOUSE_SCROLLX = 412, MOUSE_SCROLLY = 413,
    CONTROLLER_AXIS = 100, CONTROLLER_BUTTON = 0;

struct Control {

  static std::vector<Control*> controls;

  std::string name;

  uint8_t device;
  uint16_t button;
  //float x, y, px, py;
  float x, px;
  bool useDelta;
  bool alternate;
  bool invert;

  float delta() {
    return px - x;
  }

  Control(const std::string& name, uint8_t device, uint16_t button) {
    x = 0.0f;
    px = 0.0f;
    invert = false;
    alternate = false;
    useDelta = false;
    this->name = name;
    this->device = device;
    this->button = button;
  }
};

struct VirtualJoystick {

  static std::vector<VirtualJoystick*> vsticks;

  std::vector<uint16_t> controls;  // there should be a better way of doing this
  std::vector<uint8_t> direction;
  std::vector<float> magnitude;

  std::string name;
  float x, px, y, py;
  bool clamp;

  void add(uint16_t index, uint8_t angle, float mag) {
    //if (Control::controls.size() <= index)
    //  return; // out of bounds
    controls.push_back(index);
    direction.push_back(angle);
    magnitude.push_back(mag);
  }

  VirtualJoystick(bool clamp, const std::string& prefix) {
    x = 0;
    y = 0;
    px = 0;
    py = 0;
    this->clamp = clamp;
    name = prefix;
    vsticks.push_back(this);
  }

};

uint16_t AddControl(const std::string& name, uint8_t device, uint16_t button);
void MouseLock(bool a);
Control* Get(uint16_t i);
uint16_t Count();
void Setup();
void Update();
uint16_t ControlIndex(const std::string& name);
float Value(uint16_t i);
float ValuePrev(uint16_t i);

void DebugPrint();
}

//TODO: possibly move to its own header

namespace Math {
void transform3f(const glm::mat4& a, const glm::vec3& b);

// Function that returns B if it is close enough to A by THRESHOLD (<=)
inline float magnet(float a, float b, float c) {
  return (glm::abs(a - b) <= c) ? b : a;
}
}

namespace Resources {

class KShader {
 public:

  static std::vector<KShader *> shaders;
  //static std::vector<uint16_t> indices;
  //static std::vector<uint16_t> loadMe;
  
  static KShader* New(const std::string& name, const std::string& src);

  std::string source, identifier;
  bool internal;
  bool loaded;
  uint16_t id;
  
  uint16_t uniformCount;
  uint16_t textureCount;
  std::string* uniforms; // Non-default
  uint8_t* uniformTypes; // stuff
  //GLuint vert;
  //GLuint frag;

  virtual void prepareMaterial(KMaterial* material) = 0;
  virtual void Load(bool a) = 0;
  virtual void Utilize(Renderer::RenderPass* pass, KMaterial* material) = 0;

 protected:
  KShader() {
  }
  
  virtual ~KShader() {
    delete[] uniforms;
    delete[] uniformTypes;
  }
};

// TODO, make this virtual and make a GL_KTexture to take out OpenGL code
class KTexture {
 public:

  static std::vector<KTexture *> textures;
  //static std::vector<uint16_t> loadMe;

  std::string source, identifier;
  uint16_t traits;
  uint16_t width, height;
  bool isInternal, isLoaded;  //, mipmapped;
  //unsigned short imageWidth, imageHeight;
  //GLint minFilter, magFilter, textureId, wrapS, wrapT;
  GLuint textureId;

  KTexture(std::string name, GLint id, uint16_t width, uint16_t height);
  KTexture(std::string name, std::string path, uint16_t trait);
  void Load(bool a);
  void Bind();
};

class Raw {
 public:
  std::istream* stream;
  std::filebuf* fb;
  std::string filepath;
  uint8_t carton;
  Raw(const std::string& file) {
    fb = new std::filebuf;
    carton = 1;  // CARTON_FOLDER
    filepath = file;
    fb->open(file.c_str(), std::ios::in);
    stream = new std::istream(fb);
  }
  ;
  ~Raw() {
    //printf("raw deleted\n");
    fb->close();
    delete stream;
    delete fb;
  }
  ;
};

void AddCarton(const std::string& path);
void Load(const std::string& name, uint8_t type);
void Setup();
void Update();
Raw* Get(const std::string& url);

}

namespace JS {

void CallFunction(const std::string& s);
void DebugObjPrint(char* str);
void Destroy();
void Eval(const char* s);
void Eval(const std::string& s);
void GlobalUpdate();
void Setup();
void Start();
void UpdateInput();

std::string* ParseShader(std::string* in, Resources::KShader& mat);

namespace ON {
size_t Parse(const std::string& s);
size_t Parse(std::ifstream* s, const std::string& path);
std::string GetString(size_t id, const std::string& key);
void GetStringArray(size_t id, const std::string& key,
                    std::vector<std::string> &in);
size_t Enter(size_t id, const std::string& key);
void GetKeys(size_t id, std::vector<std::string> &in);
void Dispose(size_t id);
}

}

namespace Debug {
void printMatrix(const glm::mat4& a);
void printWorld();
}

}
#endif /* KONDION_H_ */
