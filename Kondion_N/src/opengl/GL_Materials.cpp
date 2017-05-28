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

//KMaterial::materials

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
    std::string* frag = JS::ParseShader(s);

    if (frag[0] != "E") {

      printf("##################\nParser result:\n%s\n##################\n",
             frag->c_str());

      Renderer::CompileShader(GL_FRAGMENT_SHADER, *frag,
                                            "Temporary Monotexture shader (VERT)");
    } else {
      printf("Error with loading shader: %s\n", frag->c_str());

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
    
    
  }
}

KMaterial* KMaterial::New(const std::string& src) {
  GL_Material* mat = new GL_Material;
  mat->source = src;
  mat->internal = (src == "i");
  materials.push_back(mat);
  return mat;
}

}

}

