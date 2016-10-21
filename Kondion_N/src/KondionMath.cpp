/*
 * KondionMath.cpp
 *
 *  Created on: Oct 19, 2016
 *      Author: neal
 */


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Kondion.h"

namespace Kondion {

	namespace Math {

		//const

		void transform3f(const glm::mat4x4& a, const glm::vec3& b) {

		}

	}

	namespace Debug {

		void printMatrix(const glm::mat4x4& a) {
			//std::cout << a.basis_element(0, 0);
			const float* b = glm::value_ptr(a);
			std::cout << b[0] << "\n";
			std::cout << "-----MATRIX-----\n"
				<< b[0] << ", " << b[4] << ", " << b[8] << ", " << b[12] << "\n"
				<< b[1] << ", " << b[5] << ", " << b[9] << ", " << b[13] << "\n"
				<< b[2] << ", " << b[6] << ", " << b[10] << ", " << b[14] << "\n"
				<< b[3] << ", " << b[7] << ", " << b[11] << ", " << b[15] << "\n";
		}

	}

}
