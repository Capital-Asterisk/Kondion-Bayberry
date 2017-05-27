/*
 * GL_Kondion.h
 *
 *  Created on: Apr 22, 2017
 *      Author: neal
 */

#ifndef OPENGL_GL_KONDION_H_
#define OPENGL_GL_KONDION_H_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <istream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "../Kondion.h"

namespace Kondion {

namespace Renderer {

class GLRenderPass : public RenderPass {
 public:

  void consider(KObj_Renderable* a);
  void force(KObj_Renderable* a);
  void generate();
  void render();
  void scan();
  GLuint id(uint8_t a) {
    return ids[5];
  }
  //GLRenderPass();
  GLuint* ids;

};

}

namespace Resources {

class GL_Material : public KMaterial {
 public:
  GLuint programId;
  void Load(bool a);
  void Utilize();
  //void New(const std::string& src);
};

}

}

#endif /* OPENGL_GL_KONDION_H_ */
