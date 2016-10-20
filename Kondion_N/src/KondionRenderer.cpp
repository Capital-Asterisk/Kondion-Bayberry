/*
 * KondionRenderer.cpp
 *
 *  Created on: Aug 23, 2016
 *      Author: neal
 */

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Kondion {


	namespace Renderer {

		void Setup() {
		}

		void Three() {
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			// glCullFace(GL_BACK);
			glEnable(GL_BLEND);
			// glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_POINT_SMOOTH);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthFunc(GL_LEQUAL);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			glClearDepth(10.0f);

		}



	}
}

