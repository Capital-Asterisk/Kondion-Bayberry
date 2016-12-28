/*
 * Resources.cpp
 *
 *  Created on: Oct 22, 2016
 *      Author: neal
 */

#ifdef _WIN32
#define SEPARATOR '\\'
#else
#define SEPARATOR '/'
#endif

#include <sys/types.h>
#include <sys/stat.h>

#include <sstream>

#include <tinydir/tinydir.h>

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Kondion.h"
#include "ktt2.h"

namespace Kondion {
namespace Resources {

const unsigned char CARTON_KCA = 0, CARTON_FOLDER = 1, CARTON_ZIP = 2;

std::vector<KTexture *> KTexture::textures;

struct Carton {

  static std::vector<Carton *> cartons;

  uint8_t type;
  std::string filepath;

  bool isGame;
  std::string id;
  std::string name;
  std::string desc;
  std::string startTitle;
  std::string version;
  std::string vendor;
  std::vector<std::string> traits;

};

std::vector<Carton *> Carton::cartons;

void AddCarton(const std::string& path) {

  printf("adding carton: %s\n", path.c_str());

  // Create an empty carton resource identifier
  Carton* c = new Carton;
  Carton::cartons.insert(Carton::cartons.end(), c);
  c->filepath = path;

  // Check if the file exists
  struct stat buf;
  int st = stat(path.c_str(), &buf);

  // Check if the file exists
  if (st == 0) {
    printf("carton exists\n");
    if (buf.st_mode & S_IFDIR) {  // all sources say '&'

      //printf("carton is folder\n");

      // now get the name and stuff
      // if the path doesn't end with a separator, add one
      c->type = CARTON_FOLDER;
      if (c->filepath[c->filepath.length() - 1] != SEPARATOR)
        c->filepath += SEPARATOR;

      // Parse kondion.json, can't use Raw get yet since we don't have the name.
      std::ifstream file(c->filepath + "kondion.json");
      int json = JS::ON::Parse(&file, c->filepath + "kondion.json");

      // Start getting information
      //c->isGame = JS::ON::GetString(json, "Game") == "true";
      c->id = JS::ON::GetString(json, "Id");
      c->name = JS::ON::GetString(json, "Name");
      c->desc = JS::ON::GetString(json, "Description");
      c->version = JS::ON::GetString(json, "Version");

      std::vector<std::string> dummy;
      JS::ON::GetKeys(json, dummy);

      // Don't mind this
      if (c->isGame) {
        c->startTitle = JS::ON::GetString(json, "StartTitle");
      }

      JS::ON::Dispose(json);

      if (c->id.length() > 8 || c->id.length() < 3) {
        perror(
            ("It is recommended that a carton ID must be 3 to 8 characters\nCarton: '"
                + c->id
                + "' is considered invalid. Kondion will continue launching\n")
                .c_str());
      }
      //if (json.is_open()) {
      //while ( getline (json, line)) {
      //cout << line << '\n';
      //}
      //json.close();
      //} else std::cout << "Unable to open file";

    } else if (buf.st_mode & S_IFREG) {
      printf("carton is file\n");
      //c->type = CARTON_KCA; // or zip
    } else {
      perror(("Invalid carton: " + path + "\n").c_str());
      delete c;
      Carton::cartons.erase(Carton::cartons.end());
    }
  } else {
    perror(("Unable to access carton: " + path + "\n").c_str());
    delete c;
    Carton::cartons.erase(Carton::cartons.end());
  }
}

void Setup() {

  // this can be more efficient
  unsigned char* ktt2 = new unsigned char[32 * 32 * 3];
  unsigned int a = 0;
  unsigned int b = 0;
  for (unsigned int i = 0; i < 32; i++) {
    for (unsigned int j = 0; j < 32 * 3; j += 3) {
      a = (i * 32 * 3 + (32 * 3 - 3 - j));
      b = 32 * 32 * 4 - 4 - (i * 32 * 3 + j) / 3 * 4;
      ktt2[a + 2] = (((ktt2_data[b + 0] - 33) << 2)
          | ((ktt2_data[b + 1] - 33) >> 4));
      ktt2[a + 1] = ((((ktt2_data[b + 1] - 33) & 0xF) << 4)
          | ((ktt2_data[b + 2] - 33) >> 2));
      ktt2[a + 0] = ((((ktt2_data[b + 2] - 33) & 0x3) << 6)
          | ((ktt2_data[b + 3] - 33)));
    }
  }

  glEnable(GL_TEXTURE_2D);
  GLuint textureId;
  glGenTextures(1, &textureId);
  std::cout << "Tex: " << textureId << "\n";
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 32, 32, 0, GL_BGR, GL_UNSIGNED_BYTE,
               ktt2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  KTexture::textures.insert(KTexture::textures.end(),
                  new KTexture("k_test", textureId, 32, 32,
                  GL_LINEAR,
                               GL_NEAREST, GL_REPEAT, GL_REPEAT, false));

  Raw* f = Get("kotega_2:masterscript");

  //printf("masterscript stream open?: %i\n", f->stream);
  //while (*f->stream)
  //	std::cout << char(f->stream->get());

  std::ostringstream ostring;
  ostring << f->stream->rdbuf();

  Kondion::JS::Eval(ostring.str());

  delete f;
}

Raw* Get(const std::string& url) {

  // url is "carton:directory/directory/filename"
  // it shouldn't be nammed url.

  // find the first occourance of ':'
  int found = url.find(':');
  // then substring it, to get just the id, "carton"
  std::string id = url.substr(0, found);
  // same thing, but in reverse, "directory/directory/filename"
  std::string current = url.substr(found + 1, url.size() - 1);

  //printf("%s \n", current.c_str());

  // loop through the cartons and find a matching id
  for (unsigned int i = 0; i < Carton::cartons.size(); i++) {
    if (Carton::cartons[i]->id == id) {
      if (Carton::cartons[i]->type == CARTON_FOLDER) {

        // open the directory of the carton, list files
        tinydir_dir dir;
        tinydir_open(&dir, Carton::cartons[i]->filepath.c_str());

        // loop through all the files in the directory
        while (dir.has_next) {

          // the current file
          tinydir_file file;
          tinydir_readfile(&dir, &file);

          // print file name for fun
          printf("%s\n", file.name);

          // choose the next file, for the next loop.
          tinydir_next(&dir);

          // reusing the 'found' variable to look for a '/' in the current path
          found = current.find('/');

          // if there is no more '/' left in the current path
          if (found == -1) {
            // looking for the final file, no more dirs to enter
            if (!file.is_dir) {
              //if (file.is_reg) // i think this would support
              // symlinks and fancy things
              // get the name of the current file and remove the file extension
              std::string name(file.name);
              name = name.substr(0, name.find('.'));

              // now the name can be compared with the one in the current path.
              if (name == current) {
                // the correct resource has been found, return a new raw data.
                printf("Resource found: %s\n", file.path);
                return new Raw(std::string(file.path));
              }
            }
          } else {
            // we're looking for a directory
            if (file.is_dir) {
              // just get the name this time.
              std::string name(file.name);

              // TODO make this more efficient
              if (name == current.substr(0, found)) {
                printf("Enter directory: %s\n",
                       current.substr(0, found).c_str());

                tinydir_open(&dir, file.path);

                current = current.substr(found + 1, current.size() - 1);
                printf("entered directory, path: %s\n", current.c_str());

              }

            }

          }

        }

        tinydir_close(&dir);
      }

    }

  }

  return NULL;
}

// For internal textures
KTexture::KTexture(std::string name, GLint id, uint16_t awidth,
                   uint16_t aheight, GLint miFilter, GLint maFilter,
                   GLint awrapS, GLint awrapT, bool mipped) {
  identifier = name;
  source = "INTERNAL";
  isLoaded = true;
  isInternal = true;
  textureId = id;
  width = awidth;
  height = aheight;
  mipmapped = mipped;
  wrapS = awrapS;
  wrapT = awrapT;
  minFilter = miFilter;
  magFilter = maFilter;
}

KTexture::KTexture(std::string name, std::string path, GLint miFilter,
                   GLint maFilter, GLint awrapS, GLint awrapT) {
  identifier = name;
  source = path;
  textureId = 0;
  isLoaded = false;
  isInternal = false;
  wrapS = awrapS;
  wrapT = awrapT;
  minFilter = miFilter;
  magFilter = maFilter;
  width = 0;
  height = 0;
  mipmapped = false;
}

void KTexture::Bind() {
  glBindTexture(GL_TEXTURE_2D, textureId);
  //std::cout << "Bind: " << textureId << "\n";
}

}
}
