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

void Three(uint16_t width, uint16_t height) {

  //glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, ((GLfloat) width) / ((GLfloat) height), 0.1f, 100.0f);
  currentCamera->prespective();  // gluLookAt
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

void RenderPass::generate() {
  // ids: fboId, brightnessTexture, depth.., diffuse, norms, final
  ids = new GLuint[5];

}

GLint neat(GLuint tex, uint16_t width, uint16_t height, GLint internal, int format, GLenum thisiscppthistime) {
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, internal, width,
      height, 0, format, thisiscppthistime, NULL);

  return tex;
}

void RenderPass::render() {
  if (!ready) {
    if (!framebuffered) {
      generate();
      framebuffered = true;
    }
    //glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
    //    GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, drbId);
    glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    //ready = EXTFramebufferObject.glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) == EXTFramebufferObject.GL_FRAMEBUFFER_COMPLETE_EXT;

  }
}

RenderPass::RenderPass(uint8_t typ, uint32_t layer, uint16_t w, uint16_t h, bool autoscn) {
  type = typ;
  camera = NULL;
  ids = {};
  width = (w == 0) ? Window::GetWidth(0) : w;
  height = (h == 0) ? Window::GetHeight(0) : h;
  autoscan = autoscn;
}

}
}

