#ifndef _FACE_
#define _FACE_

#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Quad.h"
using namespace std;

class Face {
private:
	typedef struct {
		Face *top;
		Face *bottom;
		Face *left;
		Face *right;
	} AdjFace;

	bool rotateAlongX, rotateAlongY;
	float rotationAmt;
	AdjFace adjFace;
	Quad *quadsOnFace[3][3];
	void drawQuadsOnFace(GLuint *textureArray, GLfloat (&matrix)[16]);
	void drawQuad(int col, int row, GLuint *textureArray, GLfloat (&matrix)[16]);
	
	void setRotation(float aRotationAmt, bool aRotateAlongX);
	void setQuad(Quad *aQuad, int col, int row);
	Quad * getQuad(int col, int row);

public:
	Face(int aFaceNum);
	~Face();
	void setAdjFaces(Face *top, Face *bottom, Face *left, Face *right);
	void rotateToSelf(GLfloat (&matrix)[16]);
	void rotateAbout(bool clockwise);
	void drawSelf(GLuint *textureArray, GLfloat (&matrix)[16]);
};

#endif
