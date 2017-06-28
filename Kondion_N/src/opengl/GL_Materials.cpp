/*
 * GL_Materials.cpp
 *
 *  Created on: Apr 21, 2017
 *      Author: neal
 */



#include "../Kondion.h"
#include "GL_Kondion.h"

#include <sstream>
 
namespace Kondion {

namespace Resources {

GLuint GL_Material::vertId;

void GL_Material::Load(bool a) {
  if (a) {
    // Load image
    if (loaded && internal)
      return;
    Raw* r = Resources::Get(source);
    if (r == NULL) {
      printf("[TWM]: Error in loading resource\n");
      delete r;
      return;
    }

    // TODO make this more efficient
    std::ostringstream* ostring = new std::ostringstream;
    *ostring << r->stream->rdbuf();
    std::string* s = new std::string(ostring->str());
    std::string* frag = JS::ParseShader(s, *this);

    if (frag[0] != "E") {

      printf("##################\nParser result:\n%s\n##################\n",
             frag->c_str());

      fragId = Renderer::CompileShader(GL_FRAGMENT_SHADER, *frag, "[TWM] " + source);

      if (fragId != uint16_t(-1)) {

        programId = glCreateProgram();
        glAttachShader(programId, vertId);
        glAttachShader(programId, fragId);
        glLinkProgram(programId);
        glUseProgram(programId);

        printf("Location: %i\n", glGetUniformLocation(programId, "normalmode"));
        printf("Uniform count: %u\n", uniformCount);

        uniformsLocations = new GLint[11 + uniformCount];

        uniformsLocations[0] = glGetUniformLocation(programId, "id");
        uniformsLocations[1] = glGetUniformLocation(programId, "mstime");
        uniformsLocations[2] = glGetUniformLocation(programId, "scntime");
        uniformsLocations[3] = glGetUniformLocation(programId, "normalmode");

        uniformsLocations[4] = glGetUniformLocation(programId, "depth");
        //uniformsLocations[5] = glGetUniformLocation(programId, "materials");
        //uniformsLocations[6] = glGetUniformLocation(programId, "coords");
        uniformsLocations[5] = glGetUniformLocation(programId, "coormats");
        uniformsLocations[6] = glGetUniformLocation(programId, "normals");
        uniformsLocations[7] = glGetUniformLocation(programId, "mapmals");
        uniformsLocations[8] = glGetUniformLocation(programId, "bright");
        uniformsLocations[9] = glGetUniformLocation(programId, "specs");

        for (uint8_t i = 0; i < 5; i ++)
          glUniform1i(uniformsLocations[4 + i], i);

        glUniform1i(uniformsLocations[7], 3);
        
        loaded = true;

      } else {

        printf("[TWM] Error with compiling shader: %s\n", frag->c_str());
      }

    } else {
      printf("[TWM] Error with loading shader: %s\n", frag->c_str());

    }

    delete r;
    delete ostring;
    delete s;

  } else {
    // Unload shader


  }
}

void GL_Material::Utilize(Renderer::RenderPass* pass) {
  Renderer::GLRenderPass* p = static_cast<Renderer::GLRenderPass*>(pass);
  if (p->type == 0) {
    glUseProgram(programId);
    glUniform1i(uniformsLocations[0], id);
    glUniform1i(uniformsLocations[1], Kondion::TimeMs());
    glUniform1f(uniformsLocations[2], KObj_Node::worldObject->sceneTime);
    glUniform1i(uniformsLocations[3], p->normalmode);
    

  }
}

KShader* KShader::New(const std::string& name, const std::string& src) {
  printf("New KShader\n");
  GL_Material* mat = new GL_Material;
  mat->loaded = false;
  mat->identifier = name;
  mat->source = src;
  mat->internal = (src == "i");
  shaders.push_back(mat);
  return mat;
}

}

}

