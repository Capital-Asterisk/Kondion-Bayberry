/*
 * KondionMath.cpp
 *
 *  Created on: Oct 19, 2016
 *      Author: neal
 */

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Kondion.h"

namespace Kondion {

namespace Math {

//const

void transform3f(const glm::mat4x4& a, const glm::vec3& b) {

}

}

namespace Debug {

void printMatrix(const glm::mat4x4& a) {
  //std::cout << a.basis_element(0, 0);
  const float* b = glm::value_ptr(a);
  std::cout << b[0] << "\n";
  std::cout << "-----MATRIX-----\n"
      << b[0] << ", " << b[4] << ", " << b[8]  << ", " << b[12] << "\n"
      << b[1] << ", " << b[5] << ", " << b[9]  << ", " << b[13] << "\n"
      << b[2] << ", " << b[6] << ", " << b[10] << ", " << b[14] << "\n"
      << b[3] << ", " << b[7] << ", " << b[11] << ", " << b[15] << "\n";
}

void printWorld() {
  // Print the world
  printf("---- WORLD: %i objects\n", KObj::GKO_World::worldObject->treeSize);
  for (uint16_t i = 0; i < KObj::GKO_World::worldObject->treeSize; i ++) {
    KObj_Node* e = KObj_Node::all[KObj::GKO_World::worldObject->world[i]];
    printf("%s(%i): %s[%i]\n", e->getClass()->c_str(), e->depth, e->name.c_str(), e->treeSize);
  }
  printf("---- ALL NODES: %i\n", KObj_Node::all.size());
    for (uint16_t i = 0; i < KObj_Node::all.size(); i ++) {
      KObj_Node* e = KObj_Node::all[i];
      printf("%s(%i): %s[%i] par:%i\n", e->getClass()->c_str(), e->depth, e->name.c_str(), e->treeSize,
             (e->getParent() == NULL) ? -1 : e->getParent()->allIndex);
    }
}

}

}
