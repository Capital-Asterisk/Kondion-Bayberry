/*
 * PhysicsEngine.cpp
 *
 *  Created on: Mar 20, 2017
 *      Author: neal
 */

#include "Kondion.h"

namespace Kondion {

namespace Physics {

  // used for calculations
  glm::mat4 tmat4[2];
  glm::vec4 tvec4[2];
  glm::vec3 tvec3[2];


  void CubeVsInfPlane(Component::CPN_Cube& a, Component::CPN_InfinitePlane& b) {




    tmat4[0] = glm::inverse(b.offset);
    // multiply by transform and store result in temp4
    tmat4[1] = a.offset * tmat4[0];
    // use x as some variable
    tvec3[0].x = tmat4[1][3][2] - (glm::abs(tmat4[1][0][2]) + glm::abs(tmat4[1][1][2]) + glm::abs(tmat4[1][2][2])) / 2;

    printf("Eggs: %f\n", tvec3[0].x);
  }

}


namespace Component {

void CPN_Cube::testCollision(KComponent& comp, Physics::CollisionInfo& ci) {
  if (comp.getClass() == &CPN_InfinitePlane::myClass) {
    // colliding with infinite plane
    Physics::CubeVsInfPlane(*this, dynamic_cast<CPN_InfinitePlane&>(comp));
  }
}

void CPN_InfinitePlane::testCollision(KComponent& comp, Physics::CollisionInfo& ci) {
  if (comp.getClass() == &CPN_InfinitePlane::myClass) {
    // colliding with cube

  }
}

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

        Physics::CollisionInfo ci;
        KObj_Entity* terrain;

        for (size_t j = 0; j < KObj_Node::worldObject->terrain.size(); j++) {
          //printf(
          //    "Terrain: %s\n",
          //    KObj_Node::all[KObj_Node::worldObject->terrain[j]]->name.c_str());
          terrain = static_cast<KObj_Entity*>(KObj_Node::all[KObj_Node::worldObject->terrain[j]]);
          for (uint8_t k = 0; k < ent->components.size(); k ++) {
            for (uint8_t l = 0; l < terrain->components.size(); l ++) {
              ent->components[k]->testCollision(*terrain->components[l], ci);
            }
          }
        }

      }

    }

  }

}

}
