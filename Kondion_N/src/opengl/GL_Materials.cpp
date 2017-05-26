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
      return;
    }

    // TODO make this more efficient
    std::ostringstream* ostring = new std::ostringstream;
    *ostring << r->stream->rdbuf();
    std::string* s = new std::string(ostring->str());
    JS::ParseShader(s);



  } else {
    // Unload image


  }
}

void GL_Material::Utilize() {

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

