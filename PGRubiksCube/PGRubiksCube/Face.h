/*
* Name:			Desiree Trepanier
* Project:		PG2-4 - Rubik's Cube
* Description:	The faces of the cube. A face knows which quads are on it, its adjacent faces and
				how its rotation affects those faces, and how to draw itself.
*/
#ifndef _FACE_
#define _FACE_

#include <iostream>
#include <string>
#include <cstdlib>
#include <SDL.h>
#include <SDL_opengl.h>
#include "Quad.h"
using namespace std;

#define TOP 0
#define BOTTOM 1
#define LEFT 2
#define RIGHT 3

class Face {
private:
	typedef struct {
		Face *face;
		bool affectsCol;
		int colOrRowAffected;
	} AdjFace;

	int angle;
	bool rotateAlongX, rotateAlongY, rotateQuads;
	float rotationAmt;
	AdjFace *adjFaces[4];
	Quad *quadsOnFace[3][3];
	void drawQuadsOnFace(GLuint *textureArray, GLfloat (&matrix)[16]);
	void drawQuad(int col, int row, GLuint *textureArray, GLfloat (&matrix)[16]);
	void setRotation(float aRotationAmt, bool aRotateAlongX);
	void rotateQuadsAboutOrder(bool clockwise);
	void rotateQuadsAbout(AdjFace *destFace, AdjFace *srcFace, Quad *srcQuads[3][3], bool clockwise);
	void setAffectedInAdjFaces(bool topAndBottomAffectsCol,	int topColOrRowAffected,	int bottomColOrRowAffected,
							   bool leftAndRightAffectsCol,	int leftColOrRowAffected,	int rightColOrRowAffected);

	void setQuad(Quad *aQuad, int col, int row, int angle);
	Quad * getQuad(int col, int row);
	int getAngle();

public:
	Face(int aFaceNum);
	~Face();
	void setAdjFaces(Face *top, Face *bottom, Face *left, Face *right);
	void rotateToSelf(GLfloat (&matrix)[16]);
	void rotateAbout(bool clockwise);
	void drawSelf(GLuint *textureArray, GLfloat (&matrix)[16]);
};

#endif
