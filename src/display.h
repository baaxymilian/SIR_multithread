/*
 * display.h
 *
 *  Created on: 22 sty 2021
 *      Author: Admin
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <GL/glut.h>
#include "population.h"

namespace sir{

	auto displayWindow(const char*, int, int) -> void;
	auto updateWindow(int, Population&) -> void;

}

#endif /* DISPLAY_H_ */
