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
#include "ktt2.h"

namespace Kondion {

namespace Math {

//template<typename V>
//const


void transform3f(const glm::mat4x4& a, const glm::vec3& b) {

}

// Function that returns B if it is close enough to A by THRESHOLD (<=)
//V magnet(const& ) {
//inline float magnet(float a, float b, float c) {
//  return (glm::abs(a - b) <= c) ? b : a;
//}

}

namespace Debug {

void printMatrix(const glm::mat4x4& a) {
  //std::cout << a.basis_element(0, 0);
  //const float* b = glm::value_ptr(a);
  //std::cout << b[0] << "\n";
  //std::cout << "-----MATRIX-----\n"
  //    << b[0] << ", " << b[4] << ", " << b[8]  << ", " << b[12] << "\n"
  //    << b[1] << ", " << b[5] << ", " << b[9]  << ", " << b[13] << "\n"
  //    << b[2] << ", " << b[6] << ", " << b[10] << ", " << b[14] << "\n"
  //    << b[3] << ", " << b[7] << ", " << b[11] << ", " << b[15] << "\n";
  std::cout << "-----MATRIX-----\n"
        << a[0][0] << ", " << a[0][1] << ", " << a[0][2] << ", " << a[0][3] << "\n"
        << a[1][0] << ", " << a[1][1] << ", " << a[1][2] << ", " << a[1][3] << "\n"
        << a[2][0] << ", " << a[2][1] << ", " << a[2][2] << ", " << a[2][3] << "\n"
        << a[3][0] << ", " << a[3][1] << ", " << a[3][2] << ", " << a[3][3] << "\n";
}

void printWorld() {
  // Print the world
  //printf("---- WORLD: %i objects\n", KObj::GKO_World::worldObject->treeSize);
  //for (uint16_t i = 0; i < KObj::GKO_World::worldObject->treeSize; i ++) {
  //  KObj_Node* e = KObj_Node::all[KObj::GKO_World::worldObject->world[i]];
  //  printf("%s(%i): %s[%i]\n", e->getClass()->c_str(), e->depth, e->name.c_str(), e->treeSize);
  //}
  //printf("---- ALL NODES: %i\n", KObj_Node::all.size());
  //  for (uint16_t i = 0; i < KObj_Node::all.size(); i ++) {
  //    KObj_Node* e = KObj_Node::all[i];
  //    printf("%s(%u): %s[%u] par:%i\n", e->getClass()->c_str(), e->depth, e->name.c_str(), e->treeSize,
  //           (e->getParent() == NULL) ? -1 : e->getParent()->allIndex);
  //  }

  // Print the tree of objects

  printf("[KON] World: (%u + 1) objects. %u nodes exist.\n",
         KObj::GKO_World::worldObject->treeSize, KObj_Node::all.size());

  const uint8_t stackmax = 8;
  uint8_t i = 0;
  KObj_Node* stack[stackmax]; // array of pointers
  stack[0] = KObj::GKO_World::worldObject;
  bool running = true;

  // No recursives for me today
  while (running) {
    printf("%s[%s]: \"%s\"", std::string(i + 1, '#').c_str(), stack[i]->getClass(), stack[i]->name.c_str());
    if (stack[i]->getType() >= 2) {
      KObj_Oriented* thing = static_cast<KObj_Oriented*>(stack[i]);
      printf(" (%4.2f, %4.2f, %4.2f)", thing->orientation[3].x,
             thing->orientation[3].y, thing->orientation[3].z);
    }
    printf("\n");
    if (stack[i]->children.size() != 0) {
      // Current node has children, go into first child for next loop.
      stack[i + 1] = stack[i]->children[0];
      i ++;
    } else if (stack[i]->getParent() != NULL) {
      if (stack[i]->myIndex + 1 == stack[i]->getParent()->children.size()) {
        // If last child
        if (i == 1) {
          // the end
          running = false;
        } else {
          // keep going up until it's no longer last child.
          while (running &&
                  (stack[i]->myIndex + 1
                    == stack[i]->getParent()->children.size())) {
            i --;
            // If "i" goes below 0, then stop
            //printf("I: %u\n", i);
            running = (i != uint8_t(-1));
            if (!running) {
              i = 1;
            }
          }
        }
      }

      if (running)
        stack[i] = stack[i]->getParent()->children[stack[i]->myIndex + 1];

    } else {
      // only happens if world is empty...
      running = false;
    }
  }

  printf("E: %u %u %u\n", uint8_t(kondismol_data[0]), uint8_t(kondismol_data[1]), uint8_t(kondismol_data[2]));

  //delete[] stack
}

}

}
