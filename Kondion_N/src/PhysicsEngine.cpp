/*
 * PhysicsEngine.cpp
 *
 *  Created on: Mar 20, 2017
 *      Author: neal
 */

#include "Kondion.h"

namespace Kondion {

namespace Component {
void CPN_Cube::testCollision(KComponent comp) {

}

void CPN_InfinitePlane::testCollision(KComponent comp) {
}

}


namespace Physics {



}

/*
 * How to physics
 * 1. using velocity and acceleration, calculate final positions and acceleration
 *    magnitude of all moving objects. (bad)
 *
 * When calculating, solve for displacement for the specific time
 * pretty much slow down physics when something is moving fast
 *
 * Per check:
 * 1. Move every object based on velocity and acceleration.
 * 2. When a collision is detected, solve for time on contact.
 * 3. Check for more collisions in the area.
 * 4.
 *
 * (ALL BAD)
 *
 *
 *
 */

void PhysicsUpdate() {
  double timeleft;
  double steptime = Delta();
  KObj_Entity* ent;
  for (size_t i = 0; i < KObj_Node::worldObject->children.size(); i++) {
    if (KObj_Node::worldObject->children[i]->getType() == 3) {
      // or 4;
      ent = static_cast<KObj_Entity*>(KObj_Node::worldObject->children[i]);
      if (ent->physics != 0) {
        // TODO calculate checks based on velocity
        uint8_t checks = 0;

        for (uint16_t i = 0; i < KObj_Node::worldObject->forces.size(); i ++) {
          KObj::OKO_Force* f = static_cast<KObj::OKO_Force*>
            (KObj_Node::all[KObj_Node::worldObject->forces[i]]);
          ent->velocity.x -= f->orientation[2][0] * steptime * f->strength;
          ent->velocity.y -= f->orientation[2][1] * steptime * f->strength;
          ent->velocity.z -= f->orientation[2][2] * steptime * f->strength;
          //printf("vz: %f\n", velocity.z);
          //Debug::printMatrix(f->orientation);
        }

        // TODO: include acceleration
        ent->orientation[3][0] += ent->velocity.x * steptime;
        ent->orientation[3][1] += ent->velocity.y * steptime;
        ent->orientation[3][2] += ent->velocity.z * steptime;

        for (size_t j = 0; j < KObj_Node::worldObject->terrain.size(); j++) {
          //printf(
          //    "Terrain: %s\n",
          //    KObj_Node::all[KObj_Node::worldObject->terrain[j]]->name.c_str());

        }

      }

    }

  }

}

}
