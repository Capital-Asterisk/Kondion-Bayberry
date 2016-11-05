/*
 * KondionWindow.h
 *
 *  Created on: Aug 22, 2016
 *      Author: neal
 */

#ifndef KONDION_H_
#define KONDION_H_

// or Bristlecone
#define KONDION_VERSION "0.1.0 Bayberry"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>

#include <istream>
#include <iostream>
#include <vector>

namespace Kondion {

	// Base kobj classes

	class KComponent; // avoiding a circular dependency

	// Node, type 0
	class KObj_Node {
	public:
		std::string name;
		virtual int getType() {return 0;};
		void setParent(KObj_Node* node);
		void updateA() {};
		void updateB() {};
		virtual ~KObj_Node() {};
	protected:
		KObj_Node* parent = NULL;
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
	};

	// Entity, type 3
	class KObj_Entity: public KObj_Oriented {
	public:
		std::vector<KComponent*> components;
		int getType() {return 3;};
		void render();
		void parentTransform();
	};

	// Instance, type 4
	class KObj_Instance: public KObj_Oriented {
	public:
		int getType() {return 4;};
		void render() {};
	};

	// Component part
	class KComponent {
	public:
		bool collide;
		bool renderable;
		KObj_Entity* parent; // this caused my first circular dependency
		glm::mat4x4 offset;
		virtual void render() {};
		virtual ~KComponent() {};
	protected:

	};

	// OKO - Oriented objects

	namespace Object {

		class OKO_Camera_: public KObj_Oriented {
		public:

			glm::vec3 center;
			glm::vec3 up;
			void prespective();
			void parentTransform();
		};

	}

	namespace Component {

		class CPN_Cube: public Kondion::KComponent {
		public:
			void render();
		};

		class CPN_InfinitePlane: public Kondion::KComponent {
		public:
			void render();
		};
	}

	extern char* dir;
	extern std::vector<KObj_Node *> world;

	void Launch();
	void GameLoop();

	namespace Renderer {
		extern Object::OKO_Camera_* currentCamera;
		void Setup();
		void Three(int width, int height);
		void RenderCube(float scale);
		void RenderQuad(float width, float height);
	}

	namespace Window {

		extern GLFWwindow* w;
		int Initialize();
		int CreateWindow(int width, int height);
		int SwitchWindow(int index);
		bool Active();
		void Update();

	}

	namespace Input {
		const unsigned char
				INPUT_SYSTEM = 0,
				INPUT_VIRTUAL = 1,
				INPUT_CONTROLLER = 10;

		const unsigned short
				MOUSE_BUTTON = 400,
				MOUSE_POSX = 409,
				MOUSE_POSY = 410,
				MOUSE_INSIDE = 411,
				MOUSE_SCROLLX = 412,
				MOUSE_SCROLLY = 413,
				CONTROLLER_AXIS = 100,
				CONTROLLER_BUTTON = 0;

		struct Control {
			std::string name;
			bool alternate;
			unsigned char device;
			unsigned short button;
			//float x, y, px, py;
			float x, px;
			float delta() {
				return px - x;
			};
		};

		void AddControl(std::string name, unsigned char device, unsigned short button);
		void MouseLock(bool a);
		Control* Get(unsigned short i);
		void Setup();
		void Update();
		unsigned short ControlIndex(std::string name);

		void DebugPrint();
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

	namespace JS {
		void CallFunction(std::string s);
		void Destroy();
		void Eval(std::string s);
		void Setup();
		void UpdateInput();
	}

	namespace Debug {
		void printMatrix(const glm::mat4& a);

	}

}
#endif /* KONDION_H_ */
