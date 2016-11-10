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
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Kondion.h"
#include "ktt2.h"

namespace Kondion { namespace Resources {

	const unsigned char
		CARTON_KCA = 0,
		CARTON_FOLDER = 1,
		CARTON_ZIP = 2;

	struct Carton;

	std::vector<KTexture *> textures;
	std::vector<Carton *> cartons;

	struct Carton {
		std::string name;
		std::string filepath;
		unsigned char type;
	};

	void AddCarton(std::string path) {

		printf("adding carton: %s\n", path.c_str());

		Carton* c = new Carton;
		cartons.insert(cartons.end(), c);
		c->filepath = path;

		struct stat buf;
		int st = stat(path.c_str(), &buf);

		if (st == 0) {
			printf("carton exists\n");
			if (buf.st_mode & S_IFDIR) { // all sources say &
				printf("carton is folder\n");
				// now get the name and stuff
				// if the path doesn't end with a separator, add one
				c->type = CARTON_FOLDER;
				if (c->filepath[c->filepath.length() - 1] != SEPARATOR)
					c->filepath += SEPARATOR;
				// get name

				std::ifstream json(c->filepath + "kondion.json");
				JS::ON::Parse(&json, c->filepath + "kondion.json");

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
				cartons.erase(cartons.end());
			}
		} else {
			perror(("Unable to access carton: " + path + "\n").c_str());
			delete c;
			cartons.erase(cartons.end());
		}
	}

	void Setup() {

		// this can be more efficient
		unsigned char* ktt2 = new unsigned char[32 * 32 * 3];
		unsigned int a = 0;
		unsigned int b = 0;
		for (unsigned int i = 0; i < 32; i ++) {
			for (unsigned int j = 0; j < 32 * 3; j += 3) {
				a = (i * 32 * 3 + (32 * 3 - 3 - j));
				b = 32 * 32 * 4 - 4 - (i * 32 * 3 + j) / 3 * 4;
				ktt2[a + 2] = (((ktt2_data[b + 0] - 33) << 2) | ((ktt2_data[b + 1] - 33) >> 4));
				ktt2[a + 1] = ((((ktt2_data[b + 1] - 33) & 0xF) << 4) | ((ktt2_data[b + 2] - 33) >> 2));
				ktt2[a + 0] = ((((ktt2_data[b + 2] - 33) & 0x3) << 6) | ((ktt2_data[b + 3] - 33)));
			}
		}

		glEnable(GL_TEXTURE_2D);
		GLuint textureId;
		glGenTextures(1, &textureId);
		std::cout << "Tex: " << textureId << "\n";
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 32, 32, 0, GL_BGR, GL_UNSIGNED_BYTE, ktt2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		textures.insert(textures.end(), new KTexture(textureId, 32, 32,
				GL_LINEAR, GL_NEAREST, GL_REPEAT, GL_REPEAT, false));

	}

	std::istream* Get(const std::string& url) {



		return NULL;
	}

	// Internal
	KTexture::KTexture(GLint id, unsigned short awidth, unsigned short aheight,
				GLint miFilter, GLint maFilter, GLint awrapS, GLint awrapT, bool mipped) {
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

	KTexture::KTexture(std::string path, GLint miFilter, GLint maFilter, GLint awrapS, GLint awrapT) {
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


}}
