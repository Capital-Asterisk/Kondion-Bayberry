/*
 * KondionRenderer.cpp
 *
 *  Created on: Aug 23, 2016
 *      Author: neal
 */

#include <GL/glew.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <sstream>

#include "../Kondion.h"
#include "GL_Kondion.h"

namespace Kondion {

//std::vector<Resources::KTexture *> Resources::KTexture::textures;
std::vector<Renderer::RenderPass*> Renderer::RenderPass::passes;

namespace Renderer {

// Camera used for the current render pass
Kondion::KObj::OKO_Camera_* currentCamera;

GLuint bacon; // "plane"
GLuint grill; // "plane coordinates"

//GLuint progDeferred;
GLuint progTexnorm;
GLuint progTexnormType;
GLuint progSky;
GLuint progSkyType;
GLuint progSkyPresp;

// Pretty much render everything
void Composite() {
  // Render all render passes
  for (size_t i = 0; i < RenderPass::passes.size(); i++) {
    glBindTexture(GL_TEXTURE_2D, Resources::KTexture::textures[0]->textureId);
    RenderPass::passes[i]->render();
  }
  
  // Switch to 2D mode, and don't use any shaders
  Two(0);
  glUseProgram(0);
  
  // Bind the final layer of the first render pass
  glBindTexture(GL_TEXTURE_2D, RenderPass::passes[0]->id(5));

  // Do some transformations to put it in the middle
  glTranslatef(800 / 2, 600 / 2, 0.0f);
  //glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
  glScalef(-1.0f, -1.0f, 1.0f);
  
  // Draw the thing
  RenderQuad(800, 600);
}

void Consider(KObj_Renderable* a) {
  for (size_t i = 0; i < RenderPass::passes.size(); i++) {
    RenderPass::passes[i]->consider(a);
  }
}

// Setup some opengl stuff
void Setup() {

  // Shaders

  static char dataTexNorm[] = 
    "#version 120\n"
    "uniform int type;"
    "varying vec3 normal;"
    "varying vec4 texCoord;"
    "varying vec4 viewPos;"
    "varying vec4 worldPos;"
    "varying mat4 cuteMatrix;"

    "void main() {"
    "  gl_FragData[0] = vec4(texCoord.xy, float(type) / 255.0, 1.0);"
    "  gl_FragData[1] = vec4((normalize(mat3(gl_ModelViewMatrix) * normal)"
    "                        + 1.0) / 2, 1.0);"
    "}";

  static char dataSky[] = 
    "#version 120\n"
    
    "#define PI 3.1415926535897932384626433832795028841\n"
    
    "uniform int type;"
    "uniform mat4 invPresp;"
    
    "varying vec3 normal;"
    "varying vec4 texCoord;"
    "varying vec4 viewPos;"
    "varying vec4 worldPos;"
    "varying mat4 cuteMatrix;"
    
    "float atan2(float y, float x) {"
    "  return x == 0.0 ? sign(y) * 1.570796326 : atan(y, x);"
    "}"

    "void main() {"
    "  vec3 norm = normalize(vec3(invPresp * vec4( "
    "           (800.0 - gl_FragCoord.x) / 800 - 0.5, "
    "           (gl_FragCoord.y) / 600 - 0.5,"
    "           1.0,"
    "           1.0)));"
    "  gl_FragData[0] = vec4((atan2(norm.z, norm.x) + PI) / (PI * 2), (norm.y + 1) / 2, 1.0 / 255.0, 1.0);"
    "  gl_FragData[1] = vec4((norm + 1.0) / 2, 1.0);"
    "}";


  Resources::Raw* av = Resources::Get("kdefault:shaders/solid_col");
  std::ostringstream aaostring;
  aaostring << av->stream->rdbuf();
  //delete av;

  //Resources::Raw* af = Resources::Get("kdefault:shaders/solid_tex");
  //std::ostringstream abostring;
  //abostring << af->stream->rdbuf();
  //delete af;

  //Resources::Raw* bf = Resources::Get("kdefault:shaders/deferred");
  //std::ostringstream baostring;
  //baostring << bf->stream->rdbuf();
  //delete bf;

  //printf("abostring: %s", abostring.str().c_str());

  GLuint vertPlain = CompileShader(GL_VERTEX_SHADER, aaostring.str(),
                                      "Plain vertex (VERT)");
  GLuint fragTexNorm = CompileShader(GL_FRAGMENT_SHADER, dataTexNorm,
                                      "Tex-Coord & Normal (FRAG)");
  GLuint fragSky = CompileShader(GL_FRAGMENT_SHADER, dataSky,
                                      "Sky (FRAG)");
  //GLuint defer_frag = CompileShader(GL_FRAGMENT_SHADER, baostring.str(),
  //                                  "Temporary Deferred shader (FRAG)");

  Resources::GL_Shader::vertId = vertPlain;

  progTexnorm = glCreateProgram();
  glAttachShader(progTexnorm, vertPlain);
  glAttachShader(progTexnorm, fragTexNorm);
  glLinkProgram(progTexnorm);
  glUseProgram(progTexnorm);
  
  progTexnormType = glGetUniformLocation(progTexnorm, "type");
  
  progSky = glCreateProgram();
  glAttachShader(progSky, vertPlain);
  glAttachShader(progSky, fragSky);
  glLinkProgram(progSky);
  glUseProgram(progSky);
  
  progSkyType = glGetUniformLocation(progSky, "type");
  progSkyPresp = glGetUniformLocation(progSky, "invPresp");

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

  //std::cout << "oh yeah, " << beef << "beef\n";
}

// KLoader.java line 300?
// Compiles a shader, nothing else
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
  printf("[TWM] shader log: %s\n", &d[0]);
  if (c == 0) {
    printf("Error in compiling shader %u: %s\n", a, errorname.c_str());
    glDeleteShader(a);
    return -1;
  }
  return a;
}

float fog = 0.0f;

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

  fog += (Input::Get(Input::ControlIndex("DEBUGA"))->x
      - Input::Get(Input::ControlIndex("DEBUGB"))->x) / 100;
  //printf("fog: %f\n", fog);
  //glGetUniformiv(prog_monotex, glGetUniformLocation(prog_monotex, "type"), &30);
  //printf("uniform type: %i\n", glGetUniformLocation(prog_monotex, "color"));
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

  // this is static, so it is consistent between calls
  static GLuint beef = 0;

  // if not loaded, then load
  if (beef == 0) {
    static GLfloat interlevedDataA[] = {
        0.5,  -0.5, 0.5,  0.0,  -1.0, 0.0,  0.5,  0.25, -0.5, -0.5, 0.5,  0.0,
        -1.0, 0.0,  0.5,  0.5,  -0.5, -0.5, -0.5, 0.0,  -1.0, 0.0,  0.25, 0.5,
        -0.5, 0.5,  -0.5, 0.0,  1.0,  0.0,  0.25, 0.75, -0.5, 0.5,  0.5,  0.0,
        1.0,  0.0,  0.5,  0.75, 0.5,  0.5,  0.5,  0.0,  1.0,  0.0,  0.5,  1.0,
        0.5,  0.5,  -0.5, 1.0,  0.0,  0.0,  1.0,  0.75, 0.5,  0.5,  0.5,  1.0,
        0.0,  0.0,  0.75, 0.75, 0.5,  -0.5, 0.5,  1.0,  0.0,  0.0,  0.75, 0.5,
        0.5,  0.5,  0.5,  0.0,  0.0,  1.0,  0.75, 0.75, -0.5, 0.5,  0.5,  0.0,
        0.0,  1.0,  0.5,  0.75, -0.5, -0.5, 0.5,  0.0,  0.0,  1.0,  0.5,  0.5,
        -0.5, -0.5, 0.5,  -1.0, 0.0,  0.0,  0.5,  0.5,  -0.5, 0.5,  0.5,  -1.0,
        0.0,  0.0,  0.5,  0.75, -0.5, 0.5,  -0.5, -1.0, 0.0,  0.0,  0.25, 0.75,
        0.5,  -0.5, -0.5, 0.0,  0.0,  -1.0, 0.0,  0.5,  -0.5, -0.5, -0.5, 0.0,
        0.0,  -1.0, 0.25, 0.5,  -0.5, 0.5,  -0.5, 0.0,  0.0,  -1.0, 0.25, 0.75,
        0.5,  -0.5, -0.5, 0.0,  -1.0, 0.0,  0.25, 0.25, 0.5,  -0.5, 0.5,  0.0,
        -1.0, 0.0,  0.5,  0.25, -0.5, -0.5, -0.5, 0.0,  -1.0, 0.0,  0.25, 0.5,
        0.5,  0.5,  -0.5, 0.0,  1.0,  0.0,  0.25, 1.0,  -0.5, 0.5,  -0.5, 0.0,
        1.0,  0.0,  0.25, 0.75, 0.5,  0.5,  0.5,  0.0,  1.0,  0.0,  0.5,  1.0,
        0.5,  -0.5, -0.5, 1.0,  0.0,  0.0,  1.0,  0.5,  0.5,  0.5,  -0.5, 1.0,
        0.0,  0.0,  1.0,  0.75, 0.5,  -0.5, 0.5,  1.0,  0.0,  0.0,  0.75, 0.5,
        0.5,  -0.5, 0.5,  0.0,  0.0,  1.0,  0.75, 0.5,  0.5,  0.5,  0.5,  0.0,
        0.0,  1.0,  0.75, 0.75, -0.5, -0.5, 0.5,  0.0,  0.0,  1.0,  0.5,  0.5,
        -0.5, -0.5, -0.5, -1.0, 0.0,  0.0,  0.25, 0.5,  -0.5, -0.5, 0.5,  -1.0,
        0.0,  0.0,  0.5,  0.5,  -0.5, 0.5,  -0.5, -1.0, 0.0,  0.0,  0.25, 0.75,
        0.5,  0.5,  -0.5, 0.0,  0.0,  -1.0, 0.0,  0.75, 0.5,  -0.5, -0.5, 0.0,
        0.0,  -1.0, 0.0,  0.5,  -0.5, 0.5,  -0.5, 0.0,  0.0,  -1.0, 0.25, 0.75};

    glGenBuffers(1, &beef);
    glBindBuffer(GL_ARRAY_BUFFER, beef);
    glBufferData(GL_ARRAY_BUFFER, sizeof(interlevedDataA), interlevedDataA,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // This means, the first call isn't drawn, but loaded
    return;
  }
  
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
  printf("[KGL] Considering: %s\n", a->name.c_str());
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
  printf("[KGL] RenderPass GENERATE\n");
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

  printf("[KGL] Framebuffer complete: %u\n",
         glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
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
      
    glUseProgram(progSky);

    for (size_t i = 0; i < items.size(); i++) {
      if (!items[i]->complex) {
        glPushMatrix();
        //printf("Material: %u\n", items[i]->material);
        //glUniform1i(progTexnormType, items[i]->material + 1);
        //printf("MATERIAL: %u %s POINT: %p\n", items[i]->material, Kondion::KMaterial::materials.size(), items[i]);
        //glUniform1i(prog_monotex_id, i);
        items[i]->render();
        glPopMatrix();
      }
    }
    
    glUseProgram(progSky);

    // For the sky shader to do it's calculations
    glm::mat4 presp;
    //glGetFloatv(GL_PROJECTION_MATRIX, &presp[0][0]);
    //Debug::printMatrix(presp);
    // Remove translation
    presp = glm::lookAt(glm::vec3(0.0f), glm::vec3(currentCamera->orientation[2]), glm::vec3(currentCamera->orientation[1]));
    presp = glm::inverse(presp);
    //printf("e %i\n", progSkyPresp);
    //Debug::printMatrix(presp);
    glUniformMatrix4fv(progSkyPresp, 1, GL_FALSE, &presp[0][0]);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width, height, 0, 6.0f, -6.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDepthMask(false);
    glDisable(GL_DEPTH_TEST);

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
    
    glTranslatef(width / 2.0f, height / 2.0f, -1.0f);
    
    RenderQuad(-width, -height);
    
    glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, ids[4]);
    glActiveTexture(GL_TEXTURE4);
    glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, ids[5]);
    glActiveTexture(GL_TEXTURE5);
    glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, ids[6]);

    glDrawBuffers(1, ducky + 2);
    
    Kondion::Resources::KShader* shader;
    
    normalmode = true;
    for (uint16_t i = 0; i < Kondion::KMaterial::materials.size(); i ++) {
      shader = Resources::GL_Shader::shaders
               [KMaterial::materials[i]->shader];
      if (shader->loaded) {
        //printf("Rendering: %u\n", i);
        shader->id = i;
        static_cast<Resources::GL_Shader*>(shader)
            ->Utilize(this, KMaterial::materials[i]);
        RenderQuad(-width, -height);
      }
    }

    glDrawBuffers(2, ducky + 5);
    normalmode = false;
    for (uint16_t i = 0; i < Kondion::KMaterial::materials.size(); i ++) {
      shader = Resources::GL_Shader::shaders
               [KMaterial::materials[i]->shader];
      if (shader->loaded) {
        static_cast<Resources::GL_Shader*>(shader)
            ->Utilize(this, KMaterial::materials[i]);
        RenderQuad(-width, -height);
      }
    }

    // Jelly render is done

    for (size_t i = 0; i < items.size(); i++) {
      if (items[i]->complex) {
        glPushMatrix();
        // TODO: set uniforms, order back to front
        items[i]->render();
        glPopMatrix();
      }
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

