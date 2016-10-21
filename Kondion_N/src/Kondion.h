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

#include <iostream>
#include <vector>

namespace Kondion {

	// Base kobj classes

	class KObj_Node {
	public:
		int wurla = 0;
	protected:
		KObj_Node* parent;
	};

	class KObj_Oriented: public KObj_Node {
	public:
		glm::mat4x4 offset;
		glm::mat4x4 transform;
		int woot = 0;
		long signed int longX;
		long signed int longY;
		long signed int longZ;
		void parentTransform();
	protected:
		KObj_Oriented* parentOrient;
	};

	class KObj_Entity: public KObj_Oriented {
	public:
		int woot = 0;
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

	namespace Debug {
		void printMatrix(const glm::mat4& a);

	}

}
#endif /* KONDION_H_ */
