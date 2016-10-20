/*
 * KondionWindow.h
 *
 *  Created on: Aug 22, 2016
 *      Author: neal
 */

#ifndef KONDION_H_
#define KONDION_H_

#include <cml/cml.h>

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
		cml::vector3f center;
		cml::vector3f up;
		void prespective();
		void parentTransform();
	};

	extern std::vector<KObj_Node *> world;

	void Launch();
	void GameLoop();

	namespace Renderer {

		void Setup();
		void Three(Kondion::OKO_Camera_ cam, int width, int height);


	}

	namespace Window {
		int Initialize();
		int CreateWindow(int width, int height);
		int SwitchWindow(int index);
		bool Active();
		void Update();

	}

	namespace Math {
		void transform3f(const cml::matrix44f& a, const cml::vector4f b);

	}

}
#endif /* KONDION_H_ */
