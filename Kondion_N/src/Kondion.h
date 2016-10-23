/*
 * KondionWindow.h
 *
 *  Created on: Aug 22, 2016
 *      Author: neal
 */

#ifndef KONDION_H_
#define KONDION_H_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>

#include <istream>
#include <iostream>
#include <vector>

namespace Kondion {

	// Base kobj classes

	// Node, type 0
	class KObj_Node {
	public:
		int wurla = 0;
		virtual int getType() {return 0;};
		virtual ~KObj_Node() {};
	protected:
		KObj_Node* parent;
	};

	// GKO, type 1


	// OKO, type 2
	class KObj_Oriented: public KObj_Node {
	public:
		glm::mat4x4 offset;
		glm::mat4x4 transform;
		int woot = 0;
		int farX, farY, farZ;
		int getType() {return 2;};
		virtual void parentTransform();
		//virtual ~KObj_Oriented() {};
	protected:
		KObj_Oriented* parentOrient;
	};

	// Entity, type 3
	class KObj_Entity: public KObj_Oriented {
	public:
		int woot = 0;
		int getType() {return 3;};
	};

	// OKO - Oriented objects

	class OKO_Camera_: public KObj_Oriented {
	public:

		glm::vec3 center;
		glm::vec3 up;
		void prespective();
		void parentTransform();
	};

	extern std::vector<KObj_Node *> world;

	void Launch();
	void GameLoop();

	namespace Renderer {

		void Setup();
		void Three(OKO_Camera_* camera, int width, int height);


	}

	namespace Window {

		extern GLFWwindow* w;
		int Initialize();
		int CreateWindow(int width, int height);
		int SwitchWindow(int index);
		bool Active();
		void Update();

	}

	namespace Math {
		void transform3f(const glm::mat4& a, const glm::vec3& b);

	}

	namespace Resources {
		void Setup();
		std::istream* Get(const std::string& url);

		class KTexture {
		public:
			unsigned short width, height;
			bool isInternal, isLoaded, mipmapped;
			//unsigned short imageWidth, imageHeight;
			GLint minFilter, magFilter, textureId, wrapS, wrapT;
			std::string source;

			KTexture(GLint id, unsigned short width, unsigned short height,
					GLint miFilter, GLint maFilter, GLint awrapS, GLint awrapT, bool mipped);
			KTexture(std::string path, GLint miFilter, GLint maFilter, GLint awrapS, GLint awrapT);
			void Bind();
		};

		extern std::vector<KTexture *> textures;

	}

	namespace Debug {
		void printMatrix(const glm::mat4& a);

	}

}
#endif /* KONDION_H_ */
