/*
* Name:			Desiree Trepanier
* Project:		PG2-4 - Rubik's Cube
* Description:	The quads on the cube. A quad knows its texture, its position on the texture, the
				angle its drawn at, and how to draw itself.
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
	int texture, texCol, texRow;
	int angle, animAngle;
	bool animX, animY, animZ;

	void drawGLQuad(double offset, bool drawTex);
	void setAnimAxis(bool x, bool y, bool z);

public:
	Quad(int aTexture, int aTexCol, int aTexRow);
	~Quad();
	void addToAngle(int angleToBeAdded, int anAnimAngle, int anAnimAxis);
	void animateRotation(int anAnimAngle, int anAnimAxis);
	void drawSelf(double colLoc, double rowLoc, GLuint *textureArray, GLfloat (&matrix)[16]);
	bool isAnimating();
};

#endif
