/*
 * KondionRenderer.cpp
 *
 *  Created on: Aug 23, 2016
 *      Author: neal
 */

#include <GL/glew.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>

#include "Kondion.h"

namespace Kondion {
namespace Renderer {

// Camera used for the current render pass
Kondion::KObj::OKO_Camera_* currentCamera;

GLuint beef;
GLuint bacon;
GLuint grill;

void Consider(KObj_Renderable* a) {
  for (size_t i = 0; i < RenderPass::passes.size(); i++) {
    RenderPass::passes[i]->consider(a);

  }
}

void Setup() {

  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);
  glClearColor(0.0f, 0.0f, 0.0f, 0.5f);// Black Background
  glClearDepth(1.0f);	// Depth Buffer Setup
  glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
  glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	//

  glViewport(0, 0, 800, 600);						// Reset The Current Viewport

  glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
  glLoadIdentity();									// Reset The Projection Matrix

  // Calculate The Aspect Ratio Of The Window
  gluPerspective(45.0f, (GLfloat) 800 / (GLfloat) 600, 0.1f, 100.0f);

  glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
  glLoadIdentity();									//

  glClearColor(0.5f, 0.0f, 0.0f, 1.0f);

  // this should go back to birds
  GLfloat interlevedDataA[] = { 0.5, -0.5, 0.5, 0, -1, 0, 0.5, 0.25, -0.5, -0.5,
      0.5, 0, -1, 0, 0.5, 0.5, -0.5, -0.5, -0.5, 0, -1, 0, 0.25, 0.5, -0.5, 0.5,
      -0.5, 0, 1, 0, 0.25, 0.75, -0.5, 0.5, 0.5, 0, 1, 0, 0.5, 0.75, 0.5, 0.5,
      0.5, 0, 1, 0, 0.5, 1.0, 0.5, 0.5, -0.5, 1, 0, 0, 1.0, 0.75, 0.5, 0.5, 0.5,
      1, 0, 0, 0.75, 0.75, 0.5, -0.5, 0.5, 1, 0, 0, 0.75, 0.5, 0.5, 0.5, 0.5, 0,
      0, 1, 0.75, 0.75, -0.5, 0.5, 0.5, 0, 0, 1, 0.5, 0.75, -0.5, -0.5, 0.5, 0,
      0, 1, 0.5, 0.5, -0.5, -0.5, 0.5, -1, 0, 0, 0.5, 0.5, -0.5, 0.5, 0.5, -1,
      0, 0, 0.5, 0.75, -0.5, 0.5, -0.5, -1, 0, 0, 0.25, 0.75, 0.5, -0.5, -0.5,
      0, 0, -1, 0.0, 0.5, -0.5, -0.5, -0.5, 0, 0, -1, 0.25, 0.5, -0.5, 0.5,
      -0.5, 0, 0, -1, 0.25, 0.75, 0.5, -0.5, -0.5, 0, -1, 0, 0.25, 0.25, 0.5,
      -0.5, 0.5, 0, -1, 0, 0.5, 0.25, -0.5, -0.5, -0.5, 0, -1, 0, 0.25, 0.5,
      0.5, 0.5, -0.5, 0, 1, 0, 0.25, 1.0, -0.5, 0.5, -0.5, 0, 1, 0, 0.25, 0.75,
      0.5, 0.5, 0.5, 0, 1, 0, 0.5, 1.0, 0.5, -0.5, -0.5, 1, 0, 0, 1.0, 0.5, 0.5,
      0.5, -0.5, 1, 0, 0, 1.0, 0.75, 0.5, -0.5, 0.5, 1, 0, 0, 0.75, 0.5, 0.5,
      -0.5, 0.5, 0, 0, 1, 0.75, 0.5, 0.5, 0.5, 0.5, 0, 0, 1, 0.75, 0.75, -0.5,
      -0.5, 0.5, 0, 0, 1, 0.5, 0.5, -0.5, -0.5, -0.5, -1, 0, 0, 0.25, 0.5, -0.5,
      -0.5, 0.5, -1, 0, 0, 0.5, 0.5, -0.5, 0.5, -0.5, -1, 0, 0, 0.25, 0.75, 0.5,
      0.5, -0.5, 0, 0, -1, 0.0, 0.75, 0.5, -0.5, -0.5, 0, 0, -1, 0.0, 0.5, -0.5,
      0.5, -0.5, 0, 0, -1, 0.25, 0.75 };

  glGenBuffers(1, &beef);
  glBindBuffer(GL_ARRAY_BUFFER, beef);
  glBufferData(GL_ARRAY_BUFFER, sizeof(interlevedDataA), interlevedDataA,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //delete [] interlevedDataA;

  GLfloat interlevedDataB[] =
      { -0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 0.5, 0.5,
          0.0, 0.0, 0.0, 1.0, -0.5, 0.5, 0.0, 0.0, 0.0, 1.0, };

  glGenBuffers(1, &bacon);
  glBindBuffer(GL_ARRAY_BUFFER, bacon);
  glBufferData(GL_ARRAY_BUFFER, sizeof(interlevedDataB), interlevedDataB,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //delete [] interlevedDataB;

  GLfloat interlevedDataC[] = { 1, 1, 0, 1, 0, 0, 1, 0 };

  glGenBuffers(1, &grill);
  glBindBuffer(GL_ARRAY_BUFFER, grill);
  glBufferData(GL_ARRAY_BUFFER, sizeof(interlevedDataC), interlevedDataC,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //delete [] interlevedDataC;

  std::cout << "oh yeah, " << beef << "beef\n";
}

void Three(KObj::OKO_Camera_* c, uint16_t width, uint16_t height) {

  //glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, ((GLfloat) width) / ((GLfloat) height), 0.1f, 100.0f);
  c->prespective();  // gluLookAt
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
  glVertexPointer(3, GL_FLOAT, 32, 0);  // First object
  glNormalPointer(GL_FLOAT, 32, ((char *) NULL + (12)));  // Second
  glTexCoordPointer(2, GL_FLOAT, 32, ((char *) NULL + (24)));  // Third
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

void RenderQuad(float width, float height) {
  glPushMatrix();
  glScalef(width, -height, 1);

  glBindBuffer(GL_ARRAY_BUFFER, bacon);
  glVertexPointer(3, GL_FLOAT, 24, 0);  // First object
  glNormalPointer(GL_FLOAT, 24, ((char *) NULL + (12)));  // Second
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ARRAY_BUFFER, grill);
  glTexCoordPointer(2, GL_FLOAT, 0, 0l);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glDrawArrays(GL_QUADS, 0, 8);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  glPopMatrix();
}

GLint neat(GLuint* tex, uint16_t width, uint16_t height, GLint internal, int format, GLenum thisiscppthistime) {
  glGenTextures(1, tex);
  glBindTexture(GL_TEXTURE_2D, *tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, internal, width,
      height, 0, format, thisiscppthistime, NULL);

  return *tex;
}

void RenderPass::consider(KObj_Renderable* a) {
  printf("a: %s\n", a->name.c_str());
  // unable to multiply the two drawLayers and compare to zero. two positive unsigned ints can multiply to zero.
  if ((drawLayer != 0) && (a->drawLayer != 0) && (drawLayer & a->drawLayer == drawLayer)) {
    items.push_back(a);

  }
  //if (f.isLight()) {
  //  lights.add(f);
  //  System.out.print(f);
  //} else if ((f instanceof KObj_Renderable)
  //    && (id & ((KObj_Renderable) f).id) == id)
  //  items.add((KObj_Renderable) f);
}

void RenderPass::generate() {
  // ids: fboId, brightnessTexture, depth.., diffuse, norms, final
  ids = new GLuint[6];
  printf("GENERATE\n");
  glGenFramebuffersEXT(1, ids);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, *ids);

  neat(ids + 1, width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE); // Brightness
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
      GL_COLOR_ATTACHMENT3_EXT, GL_TEXTURE_2D,
      ids[1], 0);
  neat(ids + 2, width, height, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT); // Depth texture
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
      GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D,
      ids[2], 0);
  neat(ids + 3, width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE); // Diffuse texture
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
      GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D,
      ids[3], 0);
  neat(ids + 4, width, height, GL_RGB, GL_RGB, GL_FLOAT); // Normal texture
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
      GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D,
      ids[4], 0);
  neat(ids + 5, width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE); // Result
  //if (!pixelate) {
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //}
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
      GL_COLOR_ATTACHMENT2_EXT, GL_TEXTURE_2D,
      ids[5], 0);

}

void RenderPass::render() {
  printf("hey %i %i\n", ready, framebuffered);
  if (!ready) {
    if (!framebuffered) {
      generate();
      framebuffered = true;
    }
    //glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
    //    GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, drbId);
    ready = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) == GL_FRAMEBUFFER_COMPLETE_EXT;


  } else {
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    printf("then\n");

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ids[0]);
    GLenum ducky[] = {
        GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT,
        GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT};
    glDrawBuffers(4, ducky);

    //glClearColor(Kondion.getWorld().skyColor.x, Kondion.getWorld().skyColor.y,
    //            Kondion.getWorld().skyColor.z, Kondion.getWorld().skyColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (cameraOverride && camera != NULL)
      Three(camera, width, height);
    else
      Three(currentCamera, width, height);

  }
}

void RenderPass::scan() {

}

RenderPass::RenderPass(uint8_t typ, uint32_t layer, uint16_t w, uint16_t h, bool autoscn) {
  type = typ;
  camera = NULL;
  ids = {};
  drawLayer = layer;
  width = (w == 0) ? Window::GetWidth(0) : w;
  height = (h == 0) ? Window::GetHeight(0) : h;
  autoscan = autoscn;
  passes.push_back(this);
}

}
}

