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
// any physics function will never call another physics function
glm::mat4 tmat4[2];
glm::vec4 tvec4[2];
glm::vec3 tvec3[3];

void ApplyForce(KObj_Entity* ent, glm::vec3 position, glm::vec3 force) {

  // When an object gets hit on the side, some momentum goes into linear motion
  // and some go into rotation.
  // % of momentum that goes into rotation can be approximated with:
  //   r / (i / 2 + r) * (1 - |f dot x|)
  // r = distance from center
  // i = moment of inertia
  // f = force applied
  // x = point of impact
  // I don't really know the true answer, i thought veritasium could answer that

  // magnitude of force
  float mag = glm::length(force);

  if (mag == 0 || glm::isnan(mag)) return;

  // distance from center of mass
  float dist = glm::length(position);
  // how much the force is pointed towards the center
  float dot =
      1 - glm::abs(glm::dot(glm::normalize(force), glm::normalize(position)));

  // the sigmoid thing that calculates how much percent of stuff goes into
  // angular stuff
  float amt = (dist == 0.0f) ? 0.0f : dist / (ent->radialMass / 2.0 + dist) * dot;

  // amount of rotation
  // sqrt(f/0.5i) = v
  float angVel = (amt * mag) / ent->radialMass / glm::pi<float>();

  // This mess calculates angular velocity added using mainly cross product
  // Reverse of the tangential velocity calculations
  glm::quat bird = glm::angleAxis(
      angVel * (1.0f - amt) / 32.0f,
      glm::cross(glm::normalize(position), glm::normalize(force)) *
          glm::mat3(ent->transform));

  if (amt != 0.0f) {
    ent->rotVelocity *= bird;
  }
  ent->velocity += force / ent->mass * (1.0f - amt);
}

// Detect collision between a cube and an infinite plane
void CubeVsInfPlane(Component::CPN_Cube& a, Component::CPN_InfinitePlane& b,
                    Physics::CollisionInfo& ci) {
  // printf("PAR: %s\n", a.parent->name.c_str());

  // Inverse of total transformation
  tmat4[0] = glm::inverse(b.parent->orientation * b.offset);
  
  // multiply by transform and store result in temp4
  tmat4[1] = tmat4[0] * (a.parent->orientation * a.offset);
  
  // tvec3.x is only used as a single variable, not a vector.
  // This calculates the lowest point on the cube from the matrix.
  tvec3[0].x = -(glm::abs(tmat4[1][0][2]) + glm::abs(tmat4[1][1][2]) +
                 glm::abs(tmat4[1][2][2])) /
                   2 -
               tmat4[1][3][2];

  // If the cube is inside the ground
  if (tvec3[0].x <= 0) {
    // Collision detected

    ci.collided = true;
    ci.normB = -b.offset[2];

    // t = (-v +- sqrt(v^2 + 2ad)/a) calculates time
    // derived from d = vt + 1/2at^2 using quadratic formula
    // I don't really know if this works... seems so?
    // all we need is the velocity of the cube relative to the plane
    // use the same calculations, but with velocity instead
    // TODO, maybe calculate only z
    //tvec3[1] = glm::mat3(tmat4[0]) * a.parent->velocity;
    //tvec3[2] = glm::mat3(tmat4[0]) * a.parent->acceleration;

    // z is used as a single variable.
    // This part likely doesn't work, t = (-v +- sqrt(v^2 + 2ad)/a)
    ci.collideTime =
        (-tvec3[1].z + glm::sqrt(double(tvec3[1].z * tvec3[1].z +
                                        2 * tvec3[2].z * -tvec3[0].x))) /
        tvec3[2].z;
    // printf("Collide Time: %f, (%f), e: %f a:%f \n", ci.collideTime,
    // tvec3[1].z * tvec3[1].z
    //       + 2 * tvec3[2].z * -tvec3[0].x, tvec3[2].z);

    ci.sink = tvec3[0].x;

    // Calculate point of collision on cube
    // 1. multiply B's normal [ci.normB] by A's inverse 3x3
    // 2. do rounding thing
    // 3. multiply back

    ci.spotA = glm::vec4(ci.normB * glm::mat3(a.parent->orientation), 1.0f);

    // float zero = 0.01;
    // if (glm::abs)
    // if a number is close enough to zero, then set to zero.
    ci.spotA.x = Math::magnet(ci.spotA.x, 0.0f, 0.01f);
    ci.spotA.y = Math::magnet(ci.spotA.y, 0.0f, 0.01f);
    ci.spotA.z = Math::magnet(ci.spotA.z, 0.0f, 0.01f);

    //printf("magX: %f\n", glm::length(a.offset[0]));
    //printf("magY: %f\n", glm::length(a.offset[1]));
    //printf("magZ: %f\n", glm::length(a.offset[2]));
    //float scale = glm::length(ci.spotA);

    ci.spotA = -glm::sign(ci.spotA);  // i didn't know this can be done
    ci.spotA *= glm::vec3(glm::length(a.offset[0]), glm::length(a.offset[1]), glm::length(a.offset[2]));
    //ci.spotA *= scale;

    // ci.spotA = ci.spotA * glm::mat3(a.parent->orientation * a.offset);
    ci.spotA /= 2;

    ci.spotA += glm::vec3(a.offset[3]);

    ci.spotA = ci.spotA * glm::inverse(glm::mat3(a.parent->orientation));

    // ||
    // ||||
    // |||||
    // ||||||
    // |||||||
    // |||||||
    // ||||||
    // |||||
    // ||||
    // ||

    // OwO what's this
  }
}

// Detect collision between a sphere and infinite plane
void SphereVsInfPlane(Component::CPN_Sphere& a, Component::CPN_InfinitePlane& b,
                    Physics::CollisionInfo& ci) {
  // printf("PAR: %s\n", a.parent->name.c_str());

  tmat4[0] = glm::inverse(b.parent->orientation * b.offset);
  // multiply by transform and store result in temp4
  tmat4[1] = tmat4[0] * (a.parent->orientation * a.offset);
  // use x and y as some variables, no function as a vector
  tvec3[0].y = glm::length(a.offset[0]); // Radius
  tvec3[0].x = -tvec3[0].y / 2 - tmat4[1][3][2]; // Proximity to infinite plane

  // printf("Eggs: %i %f\n", tvec3[0].x < 0, tvec3[0].x);

  if (tvec3[0].x <= 0) {
    // Collision detected
    ci.collided = true;
    ci.normB = -b.offset[2];

    tvec3[1] = glm::mat3(tmat4[0]) * a.parent->velocity;
    tvec3[2] = glm::mat3(tmat4[0]) * a.parent->acceleration;

    ci.sink = tvec3[0].x;

    ci.spotA = glm::vec4(-ci.normB * glm::mat3(a.parent->orientation), 1.0f);
    ci.spotA /= 2;
    ci.spotA += glm::vec3(a.offset[3]);
    ci.spotA = ci.spotA * glm::inverse(glm::mat3(a.parent->orientation));

  }
}

void SphereVsSphere(Component::CPN_Sphere& a, Component::CPN_Sphere& b,
                    Physics::CollisionInfo& ci) {
  // no use as a vector, only used to save ram and stuff
  tvec3[0].x = glm::length(a.offset[0]) / 2; // Radius A
  tvec3[0].y = glm::length(b.offset[0]) / 2; // Radius B

  // Vector between two spheres, used for distance calculation.
  tvec3[1] = glm::vec3((a.parent->orientation * a.offset)[3])
             - glm::vec3((b.parent->orientation * b.offset)[3]);
  if (glm::length(tvec3[1]) < tvec3[0].x + tvec3[0].y) {
    //printf("Collide!: %f\n", glm::length(tvec3[1]));
    ci.collided = true;
    ci.normA = glm::normalize(tvec3[1]);
    ci.normB = -ci.normA;
    ci.sink = (tvec3[0].x + tvec3[0].y) - glm::length(tvec3[1]);
    ci.spotA = ci.normB ;
  }
  
}

}

namespace Component {

void CPN_Cube::testCollision(KComponent& comp, Physics::CollisionInfo& ci) {
  // If infinite plane
  if (comp.getClass() == CPN_InfinitePlane::myClass) {
    // colliding with infinite plane
    Physics::CubeVsInfPlane(*this, dynamic_cast<CPN_InfinitePlane&>(comp), ci);
  }
}

void CPN_InfinitePlane::testCollision(KComponent& comp,
                                      Physics::CollisionInfo& ci) {
  if (comp.getClass() == CPN_InfinitePlane::myClass) {
    // colliding with cube
  }
}

void CPN_Sphere::testCollision(KComponent& comp,
                                      Physics::CollisionInfo& ci) {
  // Use the right function thing
  if (comp.getClass() == CPN_InfinitePlane::myClass) {
    Physics::SphereVsInfPlane(*this, dynamic_cast<CPN_InfinitePlane&>(comp), ci);
  } else if (comp.getClass() == CPN_Sphere::myClass) {
    Physics::SphereVsSphere(*this, dynamic_cast<CPN_Sphere&>(comp), ci);
  }
}
}

/*
 * How to physics
 * 1. using velocity and acceleration, calculate final positions and
 * acceleration
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

  //double timeleft;
  double steptime = Delta();

  //float dot;

  // Loop through all objects for physics
  KObj_Entity* ent;
  for (size_t i = 0; i < KObj_Node::worldObject->children.size(); i++) {
    if (KObj_Node::worldObject->children[i]->getType() == 4) {
      // or 4;
      ent = static_cast<KObj_Entity*>(KObj_Node::worldObject->children[i]);
      if (ent->physics != 0) {
        // TODO calculate checks based on velocity
        //uint8_t checks = 0;

        // Set acceleration to 0 so it doesn't add up on each other
        // TODO: there's a shorter way to do this.

        // Add up all the surrounding forces to get acceleration
        for (uint16_t i = 0; i < KObj_Node::worldObject->forces.size(); i++) {
          KObj::OKO_Force* f = static_cast<KObj::OKO_Force*>(
              KObj_Node::all[KObj_Node::worldObject->forces[i]]);
          // ent->velocity.x -= f->orientation[2][0] * steptime * f->strength;
          // ent->velocity.y -= f->orientation[2][1] * steptime * f->strength;
          // ent->velocity.z -= f->orientation[2][2] * steptime * f->strength;
          // TODO: different types of forces
          ent->acceleration.x += f->orientation[2][0] * f->strength;
          ent->acceleration.y += f->orientation[2][1] * f->strength;
          ent->acceleration.z += f->orientation[2][2] * f->strength;
          // printf("vz: %f\n", velocity.z);
          // Debug::printMatrix(f->orientation);
        }

        // glm::mat4x4 prevOrientation = glm::mat4x4(ent->orientation);

        // d = vt + 1/2 * a * t^2
        // done: include acceleration
        ent->orientation[3][0] +=
            ent->velocity.x * steptime +
            (ent->acceleration.x * (steptime * steptime)) / 2;
        ent->orientation[3][1] +=
            ent->velocity.y * steptime +
            (ent->acceleration.y * (steptime * steptime)) / 2;
        ent->orientation[3][2] +=
            ent->velocity.z * steptime +
            (ent->acceleration.z * (steptime * steptime)) / 2;

        // TODO: all this should be in a separate loop
        // except for the terrain part... which is the only one there... uwu...

        Physics::CollisionInfo ci;
        KObj_Entity* terrain;

        for (size_t j = 0; j < KObj_Node::worldObject->terrain.size(); j++) {
          // printf(
          //    "Terrain: %s, %s\n", ent->name.c_str(),
          //    KObj_Node::all[KObj_Node::worldObject->terrain[j]]->name.c_str());
          terrain = static_cast<KObj_Entity*>(
              KObj_Node::all[KObj_Node::worldObject->terrain[j]]);
          for (uint8_t k = 0; k < ent->components.size(); k++) {
            for (uint8_t l = 0; l < terrain->components.size(); l++) {
              
              if (terrain != ent) {

                // Clear ci first
                ci.collided = false;

                // Go test the collision TODO: solve random segfault

                //printf("notanerrorA: %i %i %i %s\n", k, l, terrain->components.size(), terrain->name.c_str());
                //printf("notanerrorB: %p\n", terrain->components[l]);
                ent->components[k]->testCollision(*terrain->components[l], ci);

                // Collision detected!
                if (ci.collided) {
                  if (true || steptime >= ci.collideTime - 0.001) {
                    // ent->orientation[3].x -= ci.normB.x * ci.sink;
                    // ent->orientation[3].y -= ci.normB.y * ci.sink;
                    // ent->orientation[3].z -= ci.normB.z * ci.sink;

                    // ent->orientation = prevOrientation;

                    // ent->orientation[3][0] -= ent->velocity.x * steptime
                    //    + (ent->acceleration.x * (steptime * steptime)) / 2;
                    // ent->orientation[3][1] -= ent->velocity.y * steptime
                    //    + (ent->acceleration.y * (steptime * steptime)) / 2;
                    // ent->orientation[3][2] -= ent->velocity.z * steptime
                    //    + (ent->acceleration.z * (steptime * steptime)) / 2;

                    if (precision > 2) {
                      // TODO: the response I was trying to do previously but
                      // gived up for now
                    } else {
                    
                      // Calculate normal force velocity and other stuff

                      ent->orientation[3].x += ci.normB.x * -ci.sink;
                      ent->orientation[3].y += ci.normB.y * -ci.sink;
                      ent->orientation[3].z += ci.normB.z * -ci.sink;

                      float elasticity = 0.7f;
                      float frictionMew = 2.3f;
                      

                      // Temporary stuff, remove soon
                      //glm::vec3 temp =
                      //    glm::vec3(ent->orientation
                      //              * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
                      //glm::vec3 tanVelX =
                      //    glm::cross((glm::mat3(ent->orientation)
                      //                  * glm::axis(ent->rotVelocity))
                      //               * glm::angle(ent->rotVelocity) * 32.0f,
                      //               temp);

                      // Tangental velocity at the spot
                      glm::vec3 tanVel =
                          glm::cross((glm::mat3(ent->orientation)
                                        * glm::axis(ent->rotVelocity))
                                     * glm::angle(ent->rotVelocity) * 32.0f,
                                     ci.spotA);

                      // Normal force from normal, what else?
                      glm::vec3 normalForce =
                          ci.normB *
                          ((-glm::dot(ci.normB, ent->velocity) * ent->mass +
                            glm::max(-glm::dot(ci.normB, tanVel), 0.0f) *
                                ent->radialMass) *
                           (elasticity + 1));

                      // Force needed to stop the point (momentum)
                      glm::vec3 pointForce =
                          glm::normalize(ent->velocity + tanVel)
                          * glm::length(ent->velocity * ent->mass
                                        + tanVel * ent->radialMass);

                      // Velocity flattened towards the normal
                      glm::vec3 frictionA =
                          -pointForce +
                          (ci.normB * glm::dot(ci.normB, pointForce));
                      // frictionA but with forces
                      glm::vec3 frictionB = glm::normalize(frictionA) *
                                            glm::length(normalForce) *
                                            frictionMew;
                      glm::vec3 frictionForce = frictionB;

                      if (glm::length(frictionA) == 0)
                        frictionForce = glm::vec3(0.0f);

                      // TODO Limit friction force somehow
                      // This is temporary solution
                      if (glm::length(frictionA) - glm::length(frictionB) < 0.0f)
                        //printf("EEEEEEE%f\n",
                        //       glm::length(frictionA) - glm::length(frictionB));
                        frictionForce = frictionA;

                      //printf("thing: %4.2f (%4.2f, %4.2f, %4.2f)\n",
                      //       glm::length(tanVel), tanVel.x, tanVel.y, tanVel.z);

                      // Move the debug object(s) to something

                      frictionB = -(tanVel - ent->velocity) +
                          (ci.normB * glm::dot(ci.normB, (tanVel - ent->velocity)));

                      static_cast<KObj_Entity*>(KObj_Node::all[5])
                          ->orientation[3]
                          .x = ci.spotA.x + ent->orientation[3].x - frictionB.x;
                      static_cast<KObj_Entity*>(KObj_Node::all[5])
                          ->orientation[3]
                          .y = ci.spotA.y + ent->orientation[3].y - frictionB.y;
                      static_cast<KObj_Entity*>(KObj_Node::all[5])
                          ->orientation[3]
                          .z = ci.spotA.z + ent->orientation[3].z - frictionB.z;

                      static_cast<KObj_Entity*>(KObj_Node::all[6])
                          ->orientation[3]
                          .x = ci.spotA.x + ent->orientation[3].x;
                      static_cast<KObj_Entity*>(KObj_Node::all[6])
                          ->orientation[3]
                          .y = ci.spotA.y + ent->orientation[3].y;
                      static_cast<KObj_Entity*>(KObj_Node::all[6])
                          ->orientation[3]
                          .z = ci.spotA.z + ent->orientation[3].z;

                      // printf("F: %4.2f %4.2f (%4.2f, %4.2f, %4.2f)\n",
                      //        tanVel, glm::length(force), force.x, force.y,
                      //        force.z);

                      Physics::ApplyForce(ent, ci.spotA,
                                          normalForce + frictionForce);
                      // Physics::ApplyForce(ent, ci.spotA, ci.normB);
                    }
                  }
                }
              }
            }
          }
        }

        ent->velocity.x += ent->acceleration.x * steptime;
        ent->velocity.y += ent->acceleration.y * steptime;
        ent->velocity.z += ent->acceleration.z * steptime;

        ent->acceleration.x = 0;
        ent->acceleration.y = 0;
        ent->acceleration.z = 0;
      }
    }
  }
}
}

