/*
* Name:			Desiree Trepanier
* Project:		PG2-4 - Rubik's Cube
* Description:	
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

class Face {
private:
	typedef struct {
		Face *face;
		bool affectsCol;
		int colOrRowAffected;
	} AdjFace;

	bool rotateAlongX, rotateAlongY;
	float rotationAmt;
	//Face *adjTop, *adjBottom, *adjLeft, *adjRight; //TC
	void setAffectedInAdjFaces(bool topAndBottomAffectsCol,	int topColOrRowAffected,	int bottomColOrRowAffected,
							   bool leftAndRightAffectsCol,	int leftColOrRowAffected,	int rightColOrRowAffected);
	AdjFace *adjTop, *adjBottom, *adjLeft, *adjRight;
	Quad *quadsOnFace[3][3];

	void drawQuadsOnFace(GLuint *textureArray, GLfloat (&matrix)[16]);
	void drawQuad(int col, int row, GLuint *textureArray, GLfloat (&matrix)[16]);
	void rotateQuadsAbout(AdjFace *srcFace, AdjFace *destFace, AdjFace *destFaceCCW, Quad *srcQuads[3][3], bool clockwise);
	
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
