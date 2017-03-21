/*
 * PhysicsEngine.cpp
 *
 *  Created on: Mar 20, 2017
 *      Author: neal
 */

#include "Kondion.h"

namespace Kondion {


namespace Physics {



}

/*
 * How to physics
 * 1.
 *
 *
 */

void PhysicsUpdate() {
  KObj_Entity* ent;
  for (size_t i = 0; i < KObj_Node::worldObject->children.size(); i++) {
    if (KObj_Node::worldObject->children[i]->getType() == 3) {
      // or 4;
      // TODO calculate checks based on velocity

    }
    uint8_t checks = 0;

  }
}

}
