#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Kondion.h"

namespace Kondion {

KObj_Node::KObj_Node() {
  jsObject = NULL;
  jsHidden = NULL;
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
  bool entity = this->getType() == 4;

  if (this->getType() == 4 || this->getType() == 3) {
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
      // I am entity
      //if (static_cast<KObj_Entity*>(this)->physics == 0)
      world->terrain.push_back(allIndex);
      //else
      //  world->terrain
    } else if (getClass() == KObj::OKO_Force::myClass) {
      // i am a force, add to forces
      world->forces.push_back(allIndex);
    }
    //KObj::GKO_World::worldObject->world.push_back(allIndex);

  }
  //Debug::printWorld();
}

void KObj_Entity::parentTransform() {
  KObj_Oriented::parentTransform();
  for (uint16_t i; i < components.size(); i++) {
    components[i]->parent = this;
  }
}

void KObj_Entity::render() {
  glMultMatrixf(glm::value_ptr(transform));
  for (uint16_t i; i < components.size(); i++) {
    if (components[i]->renderable) {
      components[i]->render();
    } else {
      // do something
    }
  }
}

void KObj_Entity::updateA() {
  rotVelocity.w *= 1.02f;
  rotVelocity = glm::normalize(rotVelocity);
  orientation = orientation * glm::toMat4(glm::mix(
          glm::quat(glm::vec3(0.0, 0.0, 0.0)),
          rotVelocity, float(Kondion::Delta()) * 32));
  if (physics != 0) {
    mass = 2.0f;
    radialMass = mass / 12.0f; // moment of inertia for a 1x1 cube
  }
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


}
