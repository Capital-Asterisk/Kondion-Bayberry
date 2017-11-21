/*
 * CPN.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: neal
 */

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "../Kondion.h"
#include "../opengl/GL_Kondion.h"

namespace Kondion {

namespace Component {

void CPN_Cube::render() {
  //glBindTexture(GL_TEXTURE_2D, 7);
  glPushMatrix();
  glMultMatrixf(glm::value_ptr(offset));
  Kondion::Renderer::RenderCube(1.0f);
  //parent->material = 1;
  
  //glTranslatef(0, -1, 0);
  //Kondion::Renderer::RenderQuad(20, 20);
  glPopMatrix();
  //std::cout << "meat\n";
}

void CPN_InfinitePlane::render() {
  glPushMatrix();
  glMultMatrixf(glm::value_ptr(offset));
  int amt = 32;
  float size = 8.0;

  glm::vec3 eye = glm::vec3(Kondion::Renderer::currentCamera->transform[3]);
  eye = glm::inverse(offset) * glm::vec4(eye, 1);
  //glm::vec3
  glTranslatef(glm::round(eye.x / size) * size, glm::round(eye.y / size) * size,
               0);
  glTranslatef(-size * amt / 2 - size / 2, -size * amt / 2 - size / 2, 0);
  for (int i = 0; i < amt; i++) {
    glTranslatef(size, 0, 0);
    for (int j = 0; j < amt; j++) {
      glTranslatef(0, size, 0);
      //if (glm::distance(glm::vec2(eye.x, eye.y),
      //		glm::vec2(-size * amt / 2 - size / 2 + size * i,
      //		-size * amt / 2 - size / 2 + size * j))
      //		< 2) {

      Kondion::Renderer::RenderQuad(size, -size);
      //}

    }
    glTranslatef(0, -size * amt, 0);
  }

  glPopMatrix();
  //std::cout << "meat\n";
}

void CPN_Sphere::render() {
  //glBindTexture(GL_TEXTURE_2D, 7);
  glPushMatrix();
  glMultMatrixf(glm::value_ptr(offset));
  Kondion::Renderer::RenderCube(1.0f);
  //parent->material = 1;
  
  //glTranslatef(0, -1, 0);
  //Kondion::Renderer::RenderQuad(20, 20);
  glPopMatrix();
  //std::cout << "meat\n";
}

void CPN_Mesh::render() {
  //glBindTexture(GL_TEXTURE_2D, 7);
  glPushMatrix();
  glMultMatrixf(glm::value_ptr(offset));
  Kondion::Renderer::RenderCube(1.0f);

  Resources::GL_Mesh* meal = static_cast<Resources::GL_Mesh*>(
                                Resources::KMesh::meshes[0]);

  // count: How many values are there
  // offset: Starting byte of first value
  // size: how much bytes each section takes (entire vector)
  // stride: How far apart each value is (from first byte)
  // type: What kind, and how large each value is (single value)
  // which: which buffer
  //data(Normal,Vertex,Coords)

  glBindBuffer(GL_ARRAY_BUFFER, meal->glBuffers[meal->dataVertex.which]);
  glVertexPointer(meal->dataVertex.size, GL_FLOAT, meal->dataVertex.stride,
                  (char *) NULL + meal->dataVertex.offset);
  glNormalPointer(GL_FLOAT, meal->dataNormal.stride,
                  (char *) NULL + meal->dataNormal.offset);
  glTexCoordPointer(meal->dataCoords.size, GL_FLOAT, meal->dataCoords.stride,
                  (char *) NULL + meal->dataCoords.offset);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glDrawArrays(GL_TRIANGLES, 0, meal->dataVertex.count);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  
  
  glPopMatrix();
}

}  // namespace Components

}
