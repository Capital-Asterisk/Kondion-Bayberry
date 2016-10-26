/*
 * KondionRenderer.cpp
 *
 *  Created on: Aug 23, 2016
 *      Author: neal
 */

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Kondion.h"

namespace Kondion {


	namespace Renderer {

		GLuint beef;

		void Setup() {
			//glEnable(GL_ARRAY_BUFFER);
			std::cout << "i like beef!\n";

			std::cout << "eh, " << beef << "beef\n";
			glGenBuffers(1, &beef);
			std::cout << "owo, " << beef << "beef\n";
			glBindBuffer(GL_ARRAY_BUFFER, beef);
			std::cout << "woah, " << beef << "beef\n";

			// this should go back to birds
			GLfloat interlevedCube[] = {
					0.5, -0.5, 0.5, 0, -1, 0, 0.5, 0.25, -0.5, -0.5, 0.5, 0, -1, 0,
					0.5, 0.5, -0.5, -0.5, -0.5, 0, -1, 0, 0.25, 0.5, -0.5, 0.5, -0.5, 0, 1, 0,
					0.25, 0.75, -0.5, 0.5, 0.5, 0, 1, 0, 0.5, 0.75, 0.5, 0.5, 0.5, 0, 1, 0, 0.5,
					1.0, 0.5, 0.5, -0.5, 1, 0, 0, 1.0, 0.75, 0.5, 0.5, 0.5, 1, 0, 0, 0.75,
					0.75, 0.5, -0.5, 0.5, 1, 0, 0, 0.75, 0.5, 0.5, 0.5, 0.5, 0, 0, 1, 0.75,
					0.75, -0.5, 0.5, 0.5, 0, 0, 1, 0.5, 0.75, -0.5, -0.5, 0.5, 0, 0, 1, 0.5, 0.5,
					-0.5, -0.5, 0.5, -1, 0, 0, 0.5, 0.5, -0.5, 0.5, 0.5, -1, 0, 0, 0.5, 0.75, -0.5,
					0.5, -0.5, -1, 0, 0, 0.25, 0.75, 0.5, -0.5, -0.5, 0, 0, -1, 0.0, 0.5,
					-0.5, -0.5, -0.5, 0, 0, -1, 0.25, 0.5, -0.5, 0.5, -0.5, 0, 0, -1, 0.25,
					0.75, 0.5, -0.5, -0.5, 0, -1, 0, 0.25, 0.25, 0.5, -0.5, 0.5, 0, -1, 0, 0.5,
					0.25, -0.5, -0.5, -0.5, 0, -1, 0, 0.25, 0.5, 0.5, 0.5, -0.5, 0, 1, 0,
					0.25, 1.0, -0.5, 0.5, -0.5, 0, 1, 0, 0.25, 0.75, 0.5, 0.5, 0.5, 0, 1, 0,
					0.5, 1.0, 0.5, -0.5, -0.5, 1, 0, 0, 1.0, 0.5, 0.5, 0.5, -0.5, 1, 0, 0,
					1.0, 0.75, 0.5, -0.5, 0.5, 1, 0, 0, 0.75, 0.5, 0.5, -0.5, 0.5, 0, 0, 1,
					0.75, 0.5, 0.5, 0.5, 0.5, 0, 0, 1, 0.75, 0.75, -0.5, -0.5, 0.5, 0, 0, 1,
					0.5, 0.5, -0.5, -0.5, -0.5, -1, 0, 0, 0.25, 0.5, -0.5, -0.5, 0.5, -1, 0,
					0, 0.5, 0.5, -0.5, 0.5, -0.5, -1, 0, 0, 0.25, 0.75, 0.5, 0.5, -0.5, 0, 0, -1,
					0.0, 0.75, 0.5, -0.5, -0.5, 0, 0, -1, 0.0, 0.5, -0.5, 0.5, -0.5, 0, 0,
					-1, 0.25, 0.75
			};

			glBufferData(GL_ARRAY_BUFFER, sizeof(interlevedCube), interlevedCube, GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			std::cout << "oh yeah, " << beef << "beef\n";
		}

		void Three(OKO_Camera_* camera, int width, int height) {

			//glViewport(0, 0, width, height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45.0f,((GLfloat) width) / ((GLfloat) height), 0.1f, 100.0f);
			camera->prespective(); // gluLookAt
			glMatrixMode(GL_MODELVIEW);
			//glLoadIdentity();
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

		void RenderCube(float scale) {

			// this code almost all worked when pasted from LWJGL Kondion

			glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			glScalef(scale, scale, scale);

			// setCoords(new float[] {1, 1, 0, 1, 0, 0, 1, 0});

			// floats are equal to 4 bytes
			// "Interleaved Data"
			glBindBuffer(GL_ARRAY_BUFFER, beef);
			glVertexPointer(3, GL_FLOAT, 32, 0); // First object
			glNormalPointer(GL_FLOAT, 32, ((char *)NULL + (12))); // Second
			glTexCoordPointer(2, GL_FLOAT, 32, ((char *)NULL + (24))); // Third
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			// System.out.print("EGGUS");
			glDrawArrays(GL_TRIANGLES, 0, 36);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glPopMatrix();
		}

	}
}

