/*
 * OKO.cpp
 *
 *  Created on: Aug 24, 2016
 *      Author: neal
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "include/libplatform/libplatform.h"
#include "include/v8.h"

#include "Kondion.h"

namespace Kondion {

	void OKO_Camera_::prespective()  {

	}

	void OKO_Camera_::parentTransform() {
		KObj_Oriented::parentTransform();
		// Calculate Center
		//center.tvec3(0, 0, -6);
		//center *= transform;

	}

}
