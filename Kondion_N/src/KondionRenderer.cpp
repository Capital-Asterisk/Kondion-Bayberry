/*
 * KondionRenderer.cpp
 *
 *  Created on: Aug 23, 2016
 *      Author: neal
 */

#include <GL/glew.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>

#include <sstream>

#include "Kondion.h"

namespace Kondion {
namespace Renderer {

// Camera used for the current render pass
Kondion::KObj::OKO_Camera_* currentCamera;

GLuint beef;
GLuint bacon;
GLuint grill;

GLuint temp_prog_deferred;
GLuint temp_prog_monotex;

void Composite() {
  for (size_t i = 0; i < RenderPass::passes.size(); i++) {
    glBindTexture(GL_TEXTURE_2D, Resources::textures[0]->textureId);
    RenderPass::passes[i]->render();
  }
  Two(0);
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, RenderPass::passes[0]->id(5));
  // 3: diffuse, 4: normals 5: final
  //printf("h:\ %i\n", RenderPass::passes[0]->id(2));
  //glBindTexture(GL_TEXTURE_2D, Resources::textures[0]->textureId);
  glTranslatef(800 / 2, 600 / 2, 0.0f);
  //glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
  RenderQuad(800, 600);
}

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

  //glEnable(GL_POLYGON_SMOOTH);
  //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  //glEnable(GL_MULTISAMPLE);

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


  Resources::Raw* av = Resources::Get("kotega_2:solid_col");
  std::ostringstream aaostring;
  aaostring << av->stream->rdbuf();
  //delete av;

  Resources::Raw* af = Resources::Get("kotega_2:solid_tex");
  std::ostringstream abostring;
  abostring << af->stream->rdbuf();
  //delete af;

  Resources::Raw* bf = Resources::Get("kotega_2:deferred");
  std::ostringstream baostring;
  baostring << bf->stream->rdbuf();
  //delete bf;

  printf("abostring: %s", abostring.str().c_str());

  GLuint monotex_vert = CompileShader(GL_VERTEX_SHADER, aaostring.str(), "Temporary Monotexture shader (VERT)");
  GLuint monotex_frag = CompileShader(GL_FRAGMENT_SHADER, abostring.str(), "Temporary Monotexture shader (FRAG)");
  GLuint defer_frag = CompileShader(GL_FRAGMENT_SHADER, baostring.str(), "Temporary Deferred shader (FRAG)");

  temp_prog_monotex = glCreateProgram();
  glAttachShader(temp_prog_monotex, monotex_vert);
  glAttachShader(temp_prog_monotex, monotex_frag);
  glLinkProgram(temp_prog_monotex);
  glUseProgram(temp_prog_monotex);
  printf("uniform typeee: %i\n", glGetUniformLocation(temp_prog_monotex, "color"));

  temp_prog_deferred = glCreateProgram();
  glAttachShader(temp_prog_deferred, monotex_vert);
  glAttachShader(temp_prog_deferred, defer_frag);
  glLinkProgram(temp_prog_deferred);
  glUseProgram(temp_prog_deferred);

  glProgramUniform1i(temp_prog_monotex, glGetUniformLocation(temp_prog_monotex, "texture0"), 0);

  glProgramUniform4f(temp_prog_deferred, glGetUniformLocation(temp_prog_deferred, "skyColor"), 1.0f, 1.0f, 1.0f, 1.0f);
  glProgramUniform1f(temp_prog_deferred, glGetUniformLocation(temp_prog_deferred, "fog"), 0.8f);

  glProgramUniform1i(temp_prog_deferred, glGetUniformLocation(temp_prog_deferred, "texture0"), 0);
  glProgramUniform1i(temp_prog_deferred, glGetUniformLocation(temp_prog_deferred, "texture1"), 1);
  glProgramUniform1i(temp_prog_deferred, glGetUniformLocation(temp_prog_deferred, "texture2"), 2);
  glProgramUniform1i(temp_prog_deferred, glGetUniformLocation(temp_prog_deferred, "texture3"), 3);

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

// KLoader.java line 300?
GLuint CompileShader(GLenum type, const std::string& code, const std::string& errorname) {
  GLuint a = glCreateShader(type);
  const char* f = code.c_str();
  glShaderSource(a, 1, &f, NULL);
  glCompileShader(a);
  GLint b;
  GLint c;

  glGetShaderiv(a, GL_INFO_LOG_LENGTH, &b);
  glGetShaderiv(a, GL_COMPILE_STATUS, &c);

  std::vector<GLchar> d(b);

  //printf("%s")

  glGetShaderInfoLog(a, b, &b, &d[0]);
  printf("shader log: %s\n", &d[0]);
  if (c == 0) {
    printf("Error in compiling shader: %s\n", errorname.c_str());
  }
  return a;
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


  //glGetUniformiv(temp_prog_monotex, glGetUniformLocation(temp_prog_monotex, "type"), &30);
  glUseProgram(temp_prog_monotex);
  printf("uniform type: %i\n", glGetUniformLocation(temp_prog_monotex, "color"));
  glProgramUniform1i(temp_prog_monotex, glGetUniformLocation(temp_prog_monotex, "type"), 30);
  glProgramUniform4f(temp_prog_monotex, glGetUniformLocation(temp_prog_monotex, "color"), 1.0f, 1.0f, 1.0f, 1.0f);

}

void Two(uint8_t window) {
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  //glClearColor(Kondion.getWorld().clearColor.x, Kondion.getWorld().clearColor.y,
  //    Kondion.getWorld().clearColor.z, Kondion.getWorld().clearColor.w);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0, Window::GetWidth(window), Window::GetHeight(window),
      0, 6.0f, -6.0f);
  glMatrixMode(GL_MODELVIEW);
  //glScalef(1.0f, -1.0f, 1.0f);
  //GLDrawing.setCoords(new float[] {1, 1, 0, 1, 0, 0, 1, 0});
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
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
  //printf("hey %i %i\n", ready, framebuffered);
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
    //printf("then\n");

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

    bool deep = false; //depthMask != null;
    if (deep) {
      //glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
      //    GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D,
      //    ((GKO_DeferredPass) depthMask).depId, 0);
    }

    //glPushMatrix();
    //new KMat_Monotexture().bind(type);
    //KLoader.textures.get("neat").bind();
    //glTranslatef(-getCamera().actTransform.m30, -getCamera().actTransform.m31 + 10, -getCamera().actTransform.m32);
    //Kondion.km.draw();
    //glPopMatrix();

    for (size_t i = 0; i < items.size(); i++) {
      glPushMatrix();
      items[i]->render();
      glPopMatrix();
      //if (!items.get(i).killMe) {
      //  if (!items.get(i).hidden) {
      //    items.get(i).render(type, this);
      //  }
      //
      //} else {
      //  System.out.println("Remove: " + i);
      //  items.remove(i);
      //  i --;
      //}
    }

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width, height,
        0, 6.0f, -6.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDepthMask(false);
    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0); // Diffuse
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ids[3]);
    glActiveTexture(GL_TEXTURE1); // Depth
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ids[2]);
    glActiveTexture(GL_TEXTURE2); // Normals
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ids[4]);
    glActiveTexture(GL_TEXTURE3); // Brightness
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ids[1]);
    //GLDrawing.setCoords(new float[] {1, 1, 0, 1, 0, 0, 1, 0});
    glTranslatef(width / 2.0f, height / 2.0f, -1.0f);
    //for (KObj_Renderable light : lights) {
    //  ((RKO_Light) light).apply(width, height);
    //}
    glUseProgram(temp_prog_deferred);
    //glUniform4f(skyUni, Kondion.getWorld().skyColor.x, Kondion.getWorld().skyColor.y,
    //    Kondion.getWorld().skyColor.z, Kondion.getWorld().skyColor.w);
    //glUniform1f(fogUni, 1f);
    RenderQuad(width, height);
    glTranslatef(-width / 2, -height / 2, 0);
    //KShader.unbind();
    glUseProgram(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);

    glDepthMask(true);

    glViewport(0, 0, Window::GetWidth(0), Window::GetHeight(0));

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

