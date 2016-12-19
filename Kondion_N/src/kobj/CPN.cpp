/*
 * CPN.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: neal
 */

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>


#include "../Kondion.h"

namespace Kondion {

	namespace Component {

		void CPN_Cube::render() {
			glPushMatrix();
			glMultMatrixf(glm::value_ptr(offset));
			Kondion::Renderer::RenderCube(0.5f);
			//glTranslatef(0, -1, 0);
			//Kondion::Renderer::RenderQuad(20, 20);
			glPopMatrix();
			//std::cout << "meat\n";
		}

		void CPN_InfinitePlane::render() {
			glPushMatrix();
			glMultMatrixf(glm::value_ptr(offset));
			int amt = 24;
			float size = 8.0;

			glm::vec3 eye = glm::vec3(Kondion::Renderer::currentCamera->transform[3]);
			eye = glm::inverse(offset) * glm::vec4(eye, 1);
			//glm::vec3
			glTranslatef(glm::round(eye.x / size) * size, glm::round(eye.y / size) * size, 0);
			glTranslatef(-size * amt / 2 - size / 2, -size * amt / 2 - size / 2, 0);
			for (unsigned int i = 0; i < amt; i ++) {
				glTranslatef(size, 0, 0);
				for (unsigned int j = 0; j < amt; j ++) {
					glTranslatef(0, size, 0);
					//if (glm::distance(glm::vec2(eye.x, eye.y),
					//		glm::vec2(-size * amt / 2 - size / 2 + size * i,
					//		-size * amt / 2 - size / 2 + size * j))
					//		< 2) {

					Kondion::Renderer::RenderQuad(size, size);
					//}

				}
				glTranslatef(0, -size * amt, 0);
			}

			glPopMatrix();
			//std::cout << "meat\n";
		}

	}  // namespace Components


}
