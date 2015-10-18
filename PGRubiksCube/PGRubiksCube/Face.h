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
#define X 0
#define Y 1
#define Z 2

class Face {
private:
	typedef struct {
		Face *face;
		bool affectsCol, 
			 animPositiveDir,
			 reverseMove;
		int colOrRowAffected;
	} AdjFace;

	int faceNum, angle, animAngle, rotateQuadsAngle;
	bool rotateAlongX, rotateAlongY;
	float rotationAmt;
	AdjFace *adjFaces[4];
	Quad *quadsOnFace[3][3];
	
	void setRotationInformation();
	void setAffectedInAdjFaces();
	void setAnimationAndReverseInAdjFaces();

	void drawQuadsOnFace(GLuint *textureArray, GLfloat (&matrix)[16]);
	void drawQuad(int col, int row, double colLoc, double rowLoc, GLuint *textureArray, GLfloat (&matrix)[16]);

	void rotateQuadsAboutOrder(bool clockwise);
	void rotateQuadsAbout(AdjFace *destFace, AdjFace *srcFace, Quad *srcQuads[3][3], bool clockwise);
	int getQuadsRotation(bool clockwise, AdjFace *srcFace, AdjFace *destFace);
	int getAnimAngle(AdjFace *destFace);
	bool isReverseMove(bool clockwise, AdjFace *srcFace, AdjFace *destFace);

	void setQuad(Quad *aQuad, int col, int row, int angle, int animAngle, int animAxis);
	Quad * getQuad(int col, int row);
	bool isBottomFace();
	bool isBackFace();

public:
	Face(int aFaceNum);
	~Face();
	bool isAnimating();
	void setAdjFaces(Face *top, Face *bottom, Face *left, Face *right);
	void drawSelf(GLuint *textureArray, GLfloat (&matrix)[16]);
	void rotateToSelf(GLfloat (&matrix)[16]);
	void rotateAbout(bool clockwise);
};

#endif
