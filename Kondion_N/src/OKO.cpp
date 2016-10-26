/*
 * OKO.cpp
 *
 *  Created on: Aug 24, 2016
 *      Author: neal
 */

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Kondion.h"

namespace Kondion {

	void OKO_Camera_::prespective()  {
		gluLookAt(glm::value_ptr(transform)[12], glm::value_ptr(transform)[13], glm::value_ptr(transform)[14],
				center[0], center[1], center[2],
				up[0], up[1], up[2]);
	}

	void OKO_Camera_::parentTransform() {
		KObj_Oriented::parentTransform();
		// Calculate Center
		center = glm::vec3(0.0f, 0.0f, -6.0f);
		//center.x = 0.0f;
		//center.y = 0.0f;
		//center.z = -6.0f;
		center = glm::vec3(transform * glm::vec4(center, 1.0f));
		//center += glm::trans
		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;
		up = glm::vec3(glm::vec4(up, 0.0f) * transform);
		//std::cout << center.x << ", " << center.y << ", " << center.z << ", " << glm::vec4(center, 1.0f).w << " | " << up.x << ", " << up.y << ", " << up.z << "\n";
		//center.vector(0.0f, 0.0f, 6.0f);
		//center.
		//center.tvec3(0, 0, -6);
		//center *= transform;
		//Kondion::Math::transform3f(transform, center);
	}

}
