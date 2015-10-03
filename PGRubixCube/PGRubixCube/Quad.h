#ifndef _QUAD_
#define _QUAD_

#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
using namespace std;

class Quad {
public:
	Quad(int aTexture, int aTexCol, int aTexRow, int anAngle);
	~Quad();
	void addToAngle(int angleToBeAdded);
	void drawSelf(GLuint *texture);

	int getTexture();
	int getTextureColumn();
	int getTextureRow();
	int getAngle();
	void setTexture(int aTexture);
	void setTextureColumn(int aTexCol);
	void setTextureRow(int aTexRow);
	void setAngle(int anAngle);
};

#endif
