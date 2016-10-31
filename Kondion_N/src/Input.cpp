/*
 * Input.cpp
 *
 *  Created on: Oct 29, 2016
 *      Author: neal
 */

#include <GLFW/glfw3.h>

#include "Kondion.h"

namespace Kondion { namespace Input {
	/*
	 * -Input devices, adds input types:
	 * Touch screen (pointer(s))
	 * Mouse (allow multiple) (button, pointer, axis)
	 * Keyboard (buttons, text)
	 * Controller (buttons + hat, axis, sliders)
	 * midi?
	 *
	 * -Input types:
	 * Pointer - XY
	 * Axis - more XY
	 * Slider - X
	 * Button
	 *
	 * -Virtual Input devices:
	 * Virtual slider (Button up/down, axis direction)
	 * Virtual axis (up down left right or cursor)
	 * Virtual Cursor (up down left right or cursor)
	 *
	 * -Controls
	 * List of binds go somewhere
	 * vector: struct or something {buttonname, device, key/button/axis, ... }
	 * associated with js input object
	 *
	 * -Game input:
	 * All input is in numbers
	 * js: (input["key_f"].x >= 0.5) is when button is down
	 * js: (input["axis name"].y * forward) for axis
	 *
	 * device:
	 * 0: system input (mouse, keyboard, touchscreen?)
	 *     0-399: keyboard
	 *     400-407: mouse buttons
	 *     408 - scroll, x and y axis
	 *     409 - mouse position on window, subpixel
	 *     410 - 0-1 cursor in window
	 * 1: virtual input
	 * 2:
	 *
	 * 10-26: Joystick 1-16
	 */

	struct Control {
		char name[];
		unsigned char device;
		unsigned short button;
		float x, y, px, py;
	};

	std::vector<Control> controls;

	void Update() {
		for (unsigned short i; i < controls.size(); i ++) {
			controls[i].px = controls[i].x;
			controls[i].py = controls[i].y;
			if (controls[i].device == 0) {
				 // system controller
				if (controls[i].button < 400) {
					// keyboard
					controls[i].x = glfwGetKey(Window::w, controls[i].button);
					controls[i].y = 0;
				} else if (controls[i].button < 408) {
					controls[i].x = glfwGetMouseButton(Window::w, controls[i].button - 400);
					controls[i].y = 0;
				} else if (controls[i].button == 408) {

				}
			}
		}
	}
}}

