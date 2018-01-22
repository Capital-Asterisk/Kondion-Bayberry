/*
 * OKO.cpp
 *
 *  Created on: Aug 24, 2016
 *      Author: neal
 */

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "../Kondion.h"

namespace Kondion {
namespace KObj {

void OKO_Camera_::prespective() {
  gluLookAt(glm::value_ptr(transform)[12], glm::value_ptr(transform)[13],
            glm::value_ptr(transform)[14], center[0], center[1], center[2],
            up[0], up[1], up[2]);
}

void OKO_Camera_::parentTransform() {
  KObj_Oriented::parentTransform();

  center.x = -orientation[2][0] + orientation[3][0];
  center.y = -orientation[2][1] + orientation[3][1];
  center.z = -orientation[2][2] + orientation[3][2];

  up.x = orientation[1][0];
  up.y = orientation[1][1];
  up.z = orientation[1][2];

}

void OKO_Force::parentTransform() {
  KObj_Oriented::parentTransform();
}

}
}
