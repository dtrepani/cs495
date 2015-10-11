/*
* Name:			Desiree Trepanier
* Project:		PG2-4 - Rubik's Cube
* Description:	
*/
#ifndef _QUAD_
#define _QUAD_

#include <iostream>
#include <string>
#include <cstdlib>
#include <SDL.h>
#include <SDL_opengl.h>
using namespace std;

class Quad {
private:
	int texture,
		texCol,
		texRow,
		angle;

public:
	Quad(int aTexture, int aTexCol, int aTexRow);
	~Quad();
	void addToAngle(int angleToBeAdded);
	void drawSelf(GLuint *textureArray, GLfloat (&matrix)[16]);
};

#endif
