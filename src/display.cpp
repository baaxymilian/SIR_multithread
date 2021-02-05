/*
 * display.cpp
 *
 *  Created on: 23 sty 2021
 *      Author: Admin
 */

#include "display.h"
#include <iostream>

auto sir::displayWindow(const char* name, int width,int height) -> void{

	glutInitWindowSize(width, height);

    glutCreateWindow(name);
    glClearColor(0.7f, 0.7f, 0.7f, 0.7f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	displayText("Press 'S' key to start...", -0.15, 0);
	displayText("Press 'ESC' anytime to close.", -0.2, -0.1);

    glutSwapBuffers();
}

auto sir::updateWindow(int size, Population &pop) -> void{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0.0f, 0.0f, 1.0f);

	auto scale = 1.35/size;

	auto x1 = -0.80;
	auto y1 = 0.85;
	auto x2 = x1 + scale;
	auto y2 = y1 - scale;

    auto cell = sir::Cell();
    for(auto i = 0; i < size*size; i++){
    	cell = pop.getCell(i);
    	switch(cell.getState()){
		case sir::susceptible:
			glColor3f(0.0f, 0.1f, 0.7f);
			break;
		case sir::infectous:
			glColor3f(0.9f, 0.2f, 0.0f);
			break;
		case sir::recovered:
			glColor3f(0.0f, 0.6f, 0.0f);
            break;
		default:
			break;
    	}
    	glRectd(x1, y1, x2, y2);

        x1 += 1.2*scale;
    	x2 = x1 + scale;
        if((i + 1) % size == 0){
        	x1 = -0.80;
        	x2 = x1 + scale;
            y1 -= 1.2*scale;
            y2 = y1 - scale;
        }

    }

    glFlush();
    glutSwapBuffers();
}

/* displayText
	Provide text to display on window and x, y position as frame ratio 
	in window (form -1 to 1), where 0, 0 is window's centre
*/
auto sir::displayText(const char* text, double x, double y) -> void{
	glColor3f(0.1f, 0.1f, 0.1f);
	glRasterPos2f(x, y);

	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)text);
	glutSwapBuffers();
}