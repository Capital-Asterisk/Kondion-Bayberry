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
glm::vec3 tvec3[3];

// Collision between a cube and an infinite plane
void CubeVsInfPlane(Component::CPN_Cube& a, Component::CPN_InfinitePlane& b,
                    Physics::CollisionInfo& ci) {

  tmat4[0] = glm::inverse(b.parent->orientation * b.offset);
  // multiply by transform and store result in temp4
  tmat4[1] = tmat4[0] * (a.parent->orientation * a.offset);
  // use x as some variable
  tvec3[0].x = -(glm::abs(tmat4[1][0][2]) + glm::abs(tmat4[1][1][2])
      + glm::abs(tmat4[1][2][2])) / 2 - tmat4[1][3][2];

  //printf("Eggs: %i %f\n", tvec3[0].x < 0, tvec3[0].x);

  if (tvec3[0].x <= 0) {
    // Collision detected
    //a.parent->velocity.y = Kondion::Delta() * 9;
    ci.collided = true;
    ci.normB.x = -b.offset[2][0];
    ci.normB.y = -b.offset[2][1];
    ci.normB.z = -b.offset[2][2];

    printf("NormB: (%f, %f, %f)\n", ci.normB.x, ci.normB.y, ci.normB.z);

    // TODO which surface of the cube was hit?

    // Calculate displacement
    // y = tvec3[0]
    // x =

    // t = (-v +- sqrt(v^2 + 2ad)/a) calculates time
    // derived from d = vt + 1/2at^2 using quadratic formula
    // I don't really know if this works... seems so?

    // all we need is the velocity of the cube relative to the plane
    // use the same calculations, but with velocity instead
    // TODO, maybe calculate only z
    tvec3[1] = glm::mat3(tmat4[0]) * a.parent->velocity;
    tvec3[2] = glm::mat3(tmat4[0]) * a.parent->acceleration;

    // z is used for the equation... and yeah this is big... just look above.
    ci.collideTime = (-tvec3[1].z
        + glm::sqrt(
            double(
                tvec3[1].z * tvec3[1].z
                    + 2 * tvec3[2].z * -tvec3[0].x)))
        / tvec3[2].z;
    printf("Collide Time: %f, (%f), e: %f a:%f \n", ci.collideTime, tvec3[1].z * tvec3[1].z
           + 2 * tvec3[2].z * -tvec3[0].x, tvec3[2].z);

    ci.sink = tvec3[0].x;

  }
}

}

namespace Component {

void CPN_Cube::testCollision(KComponent& comp, Physics::CollisionInfo& ci) {
  if (comp.getClass() == &CPN_InfinitePlane::myClass) {
    // colliding with infinite plane
    Physics::CubeVsInfPlane(*this, dynamic_cast<CPN_InfinitePlane&>(comp), ci);
  }
}

void CPN_InfinitePlane::testCollision(KComponent& comp,
                                      Physics::CollisionInfo& ci) {
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

  // TODO do something with this
  uint8_t precision = 1;

  double timeleft;
  double steptime = Delta();

  // Loop through all objects for physics
  KObj_Entity* ent;
  for (size_t i = 0; i < KObj_Node::worldObject->children.size(); i++) {
    if (KObj_Node::worldObject->children[i]->getType() == 3) {
      // or 4;
      ent = static_cast<KObj_Entity*>(KObj_Node::worldObject->children[i]);
      if (ent->physics != 0) {
        // TODO calculate checks based on velocity
        uint8_t checks = 0;

        // Set acceleration to 0 so it doesn't add up on each other
        // TODO: there's a shorter way to do this.
        ent->acceleration.x = 0;
        ent->acceleration.y = 0;
        ent->acceleration.z = 0;

        // Add up all the surrounding forces to get acceleration
        for (uint16_t i = 0; i < KObj_Node::worldObject->forces.size(); i++) {
          KObj::OKO_Force* f =
              static_cast<KObj::OKO_Force*>(KObj_Node::all[KObj_Node::worldObject
                  ->forces[i]]);
          //ent->velocity.x -= f->orientation[2][0] * steptime * f->strength;
          //ent->velocity.y -= f->orientation[2][1] * steptime * f->strength;
          //ent->velocity.z -= f->orientation[2][2] * steptime * f->strength;
          // TODO: different types of forces
          ent->acceleration.x += f->orientation[2][0] * f->strength;
          ent->acceleration.y += f->orientation[2][1] * f->strength;
          ent->acceleration.z += f->orientation[2][2] * f->strength;
          //printf("vz: %f\n", velocity.z);
          //Debug::printMatrix(f->orientation);
        }

        //glm::mat4x4 prevOrientation = glm::mat4x4(ent->orientation);

        // d = vt + 1/2 * a * t^2
        // done: include acceleration
        ent->orientation[3][0] += ent->velocity.x * steptime
            + (ent->acceleration.x * (steptime * steptime)) / 2;
        ent->orientation[3][1] += ent->velocity.y * steptime
            + (ent->acceleration.y * (steptime * steptime)) / 2;
        ent->orientation[3][2] += ent->velocity.z * steptime
            + (ent->acceleration.z * (steptime * steptime)) / 2;

        // TODO: all this should be in a separate loop
        // except for the terrain part... which is the only one there... uwu...

        Physics::CollisionInfo ci;
        KObj_Entity* terrain;

        for (size_t j = 0; j < KObj_Node::worldObject->terrain.size(); j++) {
          //printf(
          //    "Terrain: %s\n",
          //    KObj_Node::all[KObj_Node::worldObject->terrain[j]]->name.c_str());
          terrain =
              static_cast<KObj_Entity*>(KObj_Node::all[KObj_Node::worldObject
                  ->terrain[j]]);
          for (uint8_t k = 0; k < ent->components.size(); k++) {
            for (uint8_t l = 0; l < terrain->components.size(); l++) {

              // Go test the collision
              ent->components[k]->testCollision(*terrain->components[l], ci);

              // Collision detected!
              if (ci.collided) {

                printf("Steptime: %f EEE: %i\n", steptime, steptime >= ci.collideTime);

                if (steptime >= ci.collideTime - 0.001) {

                  //ent->orientation[3].x -= ci.normB.x * ci.sink;
                  //ent->orientation[3].y -= ci.normB.y * ci.sink;
                  //ent->orientation[3].z -= ci.normB.z * ci.sink;

                  //ent->orientation = prevOrientation;

                  //ent->orientation[3][0] -= ent->velocity.x * steptime
                  //    + (ent->acceleration.x * (steptime * steptime)) / 2;
                  //ent->orientation[3][1] -= ent->velocity.y * steptime
                  //    + (ent->acceleration.y * (steptime * steptime)) / 2;
                  //ent->orientation[3][2] -= ent->velocity.z * steptime
                  //    + (ent->acceleration.z * (steptime * steptime)) / 2;


                  if (precision > 2) {
                    // TODO: the response I was trying to do previously but
                    // gived up for now
                  } else {
                    ent->orientation[3].x -= ci.normB.x * ci.sink;
                    ent->orientation[3].y -= ci.normB.y * ci.sink;
                    ent->orientation[3].z -= ci.normB.z * ci.sink;

                  }
                }
              }
            }
          }
        }


        ent->velocity.x += ent->acceleration.x * steptime;
        ent->velocity.y += ent->acceleration.y * steptime;
        ent->velocity.z += ent->acceleration.z * steptime;

      }

    }

  }

}

}
