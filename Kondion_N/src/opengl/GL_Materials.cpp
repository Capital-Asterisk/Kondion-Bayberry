/*
 * GL_Materials.cpp
 *
 *  Created on: Apr 21, 2017
 *      Author: neal
 */



#include "../Kondion.h"
#include "GL_Kondion.h"

namespace Kondion {

namespace Resources {

void GL_Material::Load(bool a) {

}

void GL_Material::Utilize() {

}

KMaterial* KMaterial::New(const std::string& src) {
  GL_Material* mat = new GL_Material;
  mat->source = src;
  mat->internal = (src == "i");
  return mat;
}

}

}

