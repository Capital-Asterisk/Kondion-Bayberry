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

#include "../Kondion.h"
#include "GL_Kondion.h"

namespace Kondion {

//std::vector<Resources::KTexture *> Resources::KTexture::textures;
std::vector<Renderer::RenderPass*> Renderer::RenderPass::passes;

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
    glBindTexture(GL_TEXTURE_2D, Resources::KTexture::textures[0]->textureId);
    RenderPass::passes[i]->render();
  }
  Two(0);
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, RenderPass::passes[0]->id(5));
  //glBindTexture(GL_TEXTURE_2D, Resources::KTexture::textures[0]->textureId);

  //printf("h:\ %i\n", RenderPass::passes[0]->id(2));
  //glBindTexture(GL_TEXTURE_2D, Resources::textures[0]->textureId);
  glTranslatef(800 / 2, 600 / 2, 0.0f);
  //glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
  glScalef(-1.0f, -1.0f, 1.0f);
  RenderQuad(800, 600);
}

void Consider(KObj_Renderable* a) {
  for (size_t i = 0; i < RenderPass::passes.size(); i++) {
    RenderPass::passes[i]->consider(a);

  }
}

void Setup() {

  // TODO Do this properly without copy-paste stuff

  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);
  glClearDepth(1.0f);  // Depth Buffer Setup
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

  Resources::Raw* av = Resources::Get("kdefault:shaders/solid_col");
  std::ostringstream aaostring;
  aaostring << av->stream->rdbuf();
  //delete av;

  Resources::Raw* af = Resources::Get("kdefault:shaders/solid_tex");
  std::ostringstream abostring;
  abostring << af->stream->rdbuf();
  //delete af;

  Resources::Raw* bf = Resources::Get("kdefault:shaders/deferred");
  std::ostringstream baostring;
  baostring << bf->stream->rdbuf();
  //delete bf;

  //printf("abostring: %s", abostring.str().c_str());

  GLuint monotex_vert = CompileShader(GL_VERTEX_SHADER, aaostring.str(),
                                      "Temporary Monotexture shader (VERT)");
  GLuint monotex_frag = CompileShader(GL_FRAGMENT_SHADER, abostring.str(),
                                      "Temporary Monotexture shader (FRAG)");
  GLuint defer_frag = CompileShader(GL_FRAGMENT_SHADER, baostring.str(),
                                    "Temporary Deferred shader (FRAG)");

  Resources::GL_Material::vertId = monotex_vert;

  temp_prog_monotex = glCreateProgram();
  glAttachShader(temp_prog_monotex, monotex_vert);
  glAttachShader(temp_prog_monotex, monotex_frag);
  glLinkProgram(temp_prog_monotex);
  glUseProgram(temp_prog_monotex);
  printf("uniform typeee: %i\n",
         glGetUniformLocation(temp_prog_monotex, "color"));

  glProgramUniform1i(temp_prog_monotex,
                     glGetUniformLocation(temp_prog_monotex, "texture0"), 0);

  temp_prog_deferred = glCreateProgram();
  glAttachShader(temp_prog_deferred, monotex_vert);
  glAttachShader(temp_prog_deferred, defer_frag);
  glLinkProgram(temp_prog_deferred);
  glUseProgram(temp_prog_deferred);

  glProgramUniform4f(temp_prog_deferred,
                     glGetUniformLocation(temp_prog_deferred, "skyColor"), 1.0f,
                     1.0f, 1.0f, 1.0f);
  glProgramUniform1f(temp_prog_deferred,
                     glGetUniformLocation(temp_prog_deferred, "fog"), 0.02f);

  glUniform1i(glGetUniformLocation(temp_prog_deferred, "texture0"), 0);
  glUniform1i(glGetUniformLocation(temp_prog_deferred, "texture1"), 1);
  glUniform1i(glGetUniformLocation(temp_prog_deferred, "texture2"), 2);
  glUniform1i(glGetUniformLocation(temp_prog_deferred, "texture3"), 3);
  glUniform1i(glGetUniformLocation(temp_prog_deferred, "texture4"), 4);
  glUniform1i(glGetUniformLocation(temp_prog_deferred, "texture5"), 5);


  //delete [] interlevedDataA;

  // Plane Vertex and Normals
  GLfloat interlevedDataB[] = {
      -0.5, -0.5, 0.0,
      0.0, 0.0, 1.0,
      0.5, -0.5, 0.0,
      0.0, 0.0, 1.0,
      0.5, 0.5, 0.0,
      0.0, 0.0, 1.0,
      -0.5, 0.5, 0.0,
      0.0, 0.0, 1.0};

  glGenBuffers(1, &bacon);
  glBindBuffer(GL_ARRAY_BUFFER, bacon);
  glBufferData(GL_ARRAY_BUFFER, sizeof(interlevedDataB), interlevedDataB,
  GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Plane Coords
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
GLuint CompileShader(GLenum type, const std::string& code,
                     const std::string& errorname) {
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
    printf("Error in compiling shader %u: %s\n", a, errorname.c_str());
    glDeleteShader(a);
    return -1;
  }
  return a;
}

float temp_fog = 0.0f;

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

  temp_fog += (Input::Get(Input::ControlIndex("DEBUGA"))->x
      - Input::Get(Input::ControlIndex("DEBUGB"))->x) / 100;
  //printf("fog: %f\n", temp_fog);
  //glGetUniformiv(temp_prog_monotex, glGetUniformLocation(temp_prog_monotex, "type"), &30);
  glUseProgram(temp_prog_monotex);
  //printf("uniform type: %i\n", glGetUniformLocation(temp_prog_monotex, "color"));
  glUniform1i(glGetUniformLocation(temp_prog_monotex, "type"), 30);
  glUniform4f(glGetUniformLocation(temp_prog_monotex, "color"), 1.0f,
                     1.0f, 1.0f, 1.0f);
}

void Two(uint8_t window) {
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  //glClearColor(Kondion.getWorld().clearColor.x, Kondion.getWorld().clearColor.y,
  //    Kondion.getWorld().clearColor.z, Kondion.getWorld().clearColor.w);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0.0f, Window::GetWidth(window), Window::GetHeight(window), 0.0f, 6.0f,
          -6.0f);
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
  glScalef(width, -height, -1.0f);

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

GLint neat(GLuint* tex, uint16_t width, uint16_t height, GLint internal,
           GLenum format, GLenum thisiscppthistime) {
  glGenTextures(1, tex);
  glBindTexture(GL_TEXTURE_2D, *tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0, format,
  //             thisiscppthistime, NULL);
  glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0, format,
                 thisiscppthistime, NULL);
  return *tex;
}

void GLRenderPass::consider(KObj_Renderable* a) {
  printf("a: %s\n", a->name.c_str());
  // unable to multiply the two drawLayers and compare to zero. two positive unsigned ints can multiply to zero.
  if ((drawLayer != 0) && (a->drawLayer != 0)
      && ((drawLayer & a->drawLayer) == drawLayer)) {
    items.push_back(a);

  }
  //if (f.isLight()) {
  //  lights.add(f);
  //  System.out.print(f);
  //} else if ((f instanceof KObj_Renderable)
  //    && (id & ((KObj_Renderable) f).id) == id)
  //  items.add((KObj_Renderable) f);
}

void GLRenderPass::generate() {
  // K21 render system requirements:
  // 0 Framebuffer Id
  // 1 Depth
  // 2 Material
  // 3 Coords
  // 4 Normal
  // 5 Mapped Normals
  // 6 Brightness
  // 7 Specular
  // 8 Blend mode
  // 9 Jelly
  // 10 Final


  // Geometry texture inputs
  // none
  // Geometry render targets (with depth)
  // 0 Coords Material
  // 1 Normals

  // Light texture inputs
  // M

  //GLint RGB =

  ids = new GLuint[12];
  printf("RenderPass GENERATE\n");
  glGenFramebuffers(1, ids);
  glBindFramebuffer(GL_FRAMEBUFFER, *ids);

  //printf("Framebuffer status: %u\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));
  //printf("complete: %u\n", GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);

  // Depth`
  neat(ids + 1, width, height, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT,
       GL_FLOAT);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         ids[1], 0);

  // Coords (RG), Materials (B)
  neat(ids + 2, width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         ids[2], 0);

  // Normals (RGB)
  neat(ids + 3, width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
                         ids[3], 0);

  // Mapped Normals (RGB)
  neat(ids + 4, width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
                         ids[4], 0);

  // Brightness (RGB)
  neat(ids + 5, width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D,
                         ids[5], 0);

  // Speculars (RGB)
  neat(ids + 6, width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D,
                         ids[6], 0);

  // Blend mode (R)
  neat(ids + 7, width, height, GL_RED, GL_RED, GL_UNSIGNED_BYTE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D,
                         ids[7], 0);

  // Jelly (RGBA)
  neat(ids + 8, width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D,
                         ids[8], 0);

  // Final (RGBA)
  neat(ids + 9, width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT7, GL_TEXTURE_2D,
                         ids[9], 0);

  printf("Framebuffer complete: %u\n",
         glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

  /*
  neat(ids + 1, width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);  // Brightness
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
  GL_COLOR_ATTACHMENT3_EXT,
                            GL_TEXTURE_2D, ids[1], 0);
  neat(ids + 2, width, height, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT,
       GL_FLOAT);  // Depth texture
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
  GL_DEPTH_ATTACHMENT_EXT,
                            GL_TEXTURE_2D, ids[2], 0);
  neat(ids + 3, width, height, GL_RGB, GL_RED, GL_BYTE);  // Diffuse texture
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
  GL_COLOR_ATTACHMENT0_EXT,
                            GL_TEXTURE_2D, ids[3], 0);
  neat(ids + 4, width, height, GL_RGB, GL_RGB, GL_BYTE);  // Normal texture
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
  GL_COLOR_ATTACHMENT1_EXT,
                            GL_TEXTURE_2D, ids[4], 0);
  neat(ids + 5, width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);  // Result
  //if (!pixelate) {
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //}
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
  GL_COLOR_ATTACHMENT2_EXT,
                            GL_TEXTURE_2D, ids[5], 0);
  */

}

void GLRenderPass::render() {
  //printf("hey %i %i\n", ready, framebuffered);
  if (!ready) {
    generate();
    //framebuffered = true;
    ready = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER)
            == GL_FRAMEBUFFER_COMPLETE;
  }

  if (ready) {
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    //printf("then\n");

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ids[0]);

    // Materials, coords, normals
    GLenum ducky[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4,
        GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };
    glDrawBuffers(2, ducky);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (cameraOverride && camera != NULL)
      Three(camera, width, height);
    else
      Three(currentCamera, width, height);

    // Render all items, TODO: render only opaque if transparency supported
    for (size_t i = 0; i < items.size(); i++) {
      glPushMatrix();
      items[i]->render();
      glPopMatrix();
    }

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width, height, 0, 6.0f, -6.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDepthMask(false);
    glDisable(GL_DEPTH_TEST);

    // 0 Depth
    // 1 Coords & materials
    // 2 Normals
    // 3 Mapped normals
    // 4 Brightness
    // 5 Specular

    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ids[1]); // depth
    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ids[2]); // coormats
    glActiveTexture(GL_TEXTURE2);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ids[3]); // normals
    glActiveTexture(GL_TEXTURE3);
    glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, ids[4]);
    glActiveTexture(GL_TEXTURE4);
    glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, ids[5]);
    glActiveTexture(GL_TEXTURE5);
    glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, ids[6]);

    //GLenum goose[] = { GL_COLOR_ATTACHMENT2 };
    //glDrawBuffers(0, NULL);
    glDrawBuffers(1, ducky + 2);
    //glUseProgram(temp_prog_deferred);
    //glUniform1f(glGetUniformLocation(temp_prog_deferred, "fog"), temp_fog);

    glTranslatef(width / 2.0f, height / 2.0f, -1.0f);

    normalmode = true;
    for (uint16_t i = 0; i < Resources::GL_Material::materials.size(); i ++) {
      static_cast<Resources::GL_Material*>(Resources::GL_Material::materials[i])
          ->Utilize(this);
      //printf("R%u\n", i);
      RenderQuad(-width, -height);
    }

    //GLenum* quack = ducky + 7;
    //GLenum quack[] = { GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7, GL_COLOR_ATTACHMENT8 };

    //glDrawBuffers(0, NULL);
    //glDrawBuffers(3, quack);
    glDrawBuffers(2, ducky + 5); // i don't know why this doesn't work

    //int eat = 0;
    //glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &eat);

    //printf("max: %i\n", eat);

    normalmode = false;
    for (uint16_t i = 0; i < Resources::GL_Material::materials.size(); i++) {
      static_cast<Resources::GL_Material*>(Resources::GL_Material::materials[i])
          ->Utilize(this);
      //printf("R%u\n", i);

      RenderQuad(-width, -height);
    }


    //glUniform4f(skyUni, Kondion.getWorld().skyColor.x, Kondion.getWorld().skyColor.y,
    //    Kondion.getWorld().skyColor.z, Kondion.getWorld().skyColor.w);
    //glUniform1f(fogUni, 1f);

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

void GLRenderPass::force(Kondion::KObj_Renderable* d) {

}


void GLRenderPass::scan() {

}

//GLRenderPass::GLRenderPass() {

//}

void RenderPass::New(uint8_t type, uint32_t drawLayer, uint16_t width, uint16_t height,
                       bool autoscan) {
  GLRenderPass* rp = new GLRenderPass;
  rp->type = type;
  rp->camera = NULL;
  rp->ids = {};
  rp->drawLayer = drawLayer;
  rp->width = (width == 0) ? Window::GetWidth(0) : width;
  rp->height = (height == 0) ? Window::GetHeight(0) : height;
  rp->autoscan = autoscan;
  passes.push_back(rp);
}

}
}

