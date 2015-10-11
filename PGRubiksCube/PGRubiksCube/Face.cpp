/*
* Name:			Desiree Trepanier
* Project:		PG2-4 - Rubik's Cube
* Description:	
*/
#include "Face.h"

static double twoThirds = 2.0/3.0;

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TODO: ROTATE QUADS WHEN ROTATE ABOUT FACE

Face::Face(int textureNum) {
	for(int i = 0; i < 3; i ++) {
		for(int j = 0; j < 3; j++) {
			quadsOnFace[i][j] = new Quad(textureNum, i, j);
		}
	}

	switch(textureNum) {
		case 0: // Top
			setRotation(-90.0, true);
			setAffectedInAdjFaces(false, 0, 0, false, 0, 0);
			break;
		case 1: // Bottom
			setRotation(90.0, true);
			setAffectedInAdjFaces(false, 2, 2, false, 2, 2);
			break;
		case 2: // Left
			setRotation(-90.0, false);
			setAffectedInAdjFaces(false, 0, 2, true, 2, 0); // TC: bottom might be 0 instead of 2
			break;
		case 3: // Front
			setRotation(0.0, false);
			setAffectedInAdjFaces(false, 2, 0, true, 2, 0);
			break;
		case 4: // Back
			setRotation(90.0, false);
			setAffectedInAdjFaces(false, 0, 2, true, 2, 0);
			break;
		case 5: // Right
			setRotation(180.0, false);
			setAffectedInAdjFaces(false, 2, 0, true, 2, 0);
			break;
	}
}

Face::~Face() {
	delete adjTop;
	delete adjBottom;
	delete adjLeft;
	delete adjRight;
	delete[] &quadsOnFace;
}

void Face::setAffectedInAdjFaces(bool topAndBottomAffectsCol,	int topColOrRowAffected,	int bottomColOrRowAffected,
								 bool leftAndRightAffectsCol,	int leftColOrRowAffected,	int rightColOrRowAffected) {
	adjTop		= new AdjFace;	adjTop->affectsCol	  = topAndBottomAffectsCol;		adjTop->colOrRowAffected	= topColOrRowAffected;
	adjBottom	= new AdjFace;	adjBottom->affectsCol = topAndBottomAffectsCol;		adjBottom->colOrRowAffected	= bottomColOrRowAffected;
	adjLeft		= new AdjFace;	adjLeft->affectsCol	  = leftAndRightAffectsCol;		adjLeft->colOrRowAffected	= leftColOrRowAffected;
	adjRight	= new AdjFace;	adjRight->affectsCol  = leftAndRightAffectsCol;		adjRight->colOrRowAffected	= rightColOrRowAffected;
}

// Each face will need to know which faces are adjacent to it when the cube is rotate is rotated about the face.
void Face::setAdjFaces(Face *aTop, Face *aBottom, Face *aLeft, Face *aRight) {
	adjTop->face = aTop;
	adjBottom->face = aBottom;
	adjLeft->face = aLeft;
	adjRight->face = aRight;
}

// Rotate to the given face in order to draw the quads.
void Face::rotateToSelf(GLfloat (&matrix)[16]) {
	glLoadMatrixf(matrix);
	glRotatef( rotationAmt, (float) rotateAlongX, (float) rotateAlongY, 0 );
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}

// Rotates the face and all quads touching the face on adjacent faces.
// The quads on the face will rotate as well as move positions.
// The quads on adjacent faces will not rotate, but will move positions and move to another face.
void Face::rotateAbout(bool clockwise) {
	int rotation = clockwise ? -90 : 90;
	Quad *quadsBeforeRotation[3][3];
	memcpy(&quadsBeforeRotation[0][0], &quadsOnFace[0][0], sizeof(quadsBeforeRotation[0][0]) * 9);

	for( int i = 0; i < 3; i++ ) {
		for( int j = 0; j < 3; j++ ) {
			quadsOnFace[i][j]->addToAngle(rotation);
		}
	}
	
	for( int i = 0; i < 3; i++ ) {
		for( int j = 0; j < 3; j++ ) {
			if( !(i == 1 && j == 1) ) { // is not section 5
				if( clockwise ) {
					quadsOnFace[2-j][i] = quadsBeforeRotation[i][j];
				} else {
					quadsOnFace[j][2-i] = quadsBeforeRotation[i][j];				
				}
			}
		}
	}

	Quad *topFaceBottomRow[3][3];
	for( int i = 0; i < 3; i++ ) {
		for( int j = 0; j < 3; j++ ) {
			topFaceBottomRow[i][j] = adjTop->face->getQuad(i, j);	
		}
	}
	
	// TC: NOT FINISHED WITH THIS STUFF
	rotateQuadsAbout(adjLeft,	adjTop,		adjTop,		NULL,				clockwise);
	rotateQuadsAbout(adjBottom,	adjLeft,	adjBottom,	NULL,				clockwise);
	rotateQuadsAbout(adjRight,	adjBottom,  adjRight,	NULL,				clockwise);
	rotateQuadsAbout(adjTop,	adjRight,	adjLeft,	topFaceBottomRow,	clockwise);
}

void Face::rotateQuadsAbout(AdjFace *srcFace, AdjFace *destFace, AdjFace *destFaceCCW, Quad *srcQuads[3][3], bool clockwise) {
	for( int i = 0; i < 3; i++ ) {
		int srcCol, srcRow, destCol, destRow;
		
		if( srcFace->affectsCol ) {
			srcCol = srcFace->colOrRowAffected;
			srcRow = i;
		} else {
			srcCol = i;
			srcRow = srcFace->colOrRowAffected;
		}

		if( destFace->affectsCol ) {
			destCol = destFace->colOrRowAffected;
			destRow = clockwise? i : 2 - i;
		} else {
			destCol = clockwise? i : 2 - i;
			destRow = destFace->colOrRowAffected;
		}

		if( srcFace != adjTop ) {
			destFace->face->setQuad( srcFace->face->getQuad( srcCol, srcRow ), destCol, destRow );
		} else {
			destFace->face->setQuad( srcQuads[srcCol][srcRow], destCol, destRow );
		}
	}
}

// Draw the underlying black quad and then the quads currently on this face.
void Face::drawSelf(GLuint *textureArray, GLfloat (&matrix)[16]) {
	glLoadMatrixf(matrix);

	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
		glVertex3f( -1,	-1,	1 );
		glVertex3f(  1, -1,	1 );
		glVertex3f(  1,  1,	1 );
		glVertex3f( -1,  1,	1 );
	glEnd();

	drawQuadsOnFace(textureArray, matrix);
	
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}

// Set the face's rotation angle and whether its on the x or y-axis.
// Allows the face to know where to rotate to get to itself.
void Face::setRotation(float aRotationAmt, bool aRotateAlongX) {
	rotationAmt = (float) aRotationAmt;
	if(aRotateAlongX) {
		rotateAlongX = true;
		rotateAlongY = false;
	} else {
		rotateAlongX = false;
		rotateAlongY = true;
	}
}

// The face draws each of the quads currently on it.
// The quads do not know how to get to themselves to draw themselves, so translation
// on the face level is necessary.
void Face::drawQuadsOnFace(GLuint *textureArray, GLfloat (&matrix)[16]) {
	glTranslatef( twoThirds * -1.0, twoThirds * -1.0, 0 );
	glColor3f(1, 1, 1);

	for( int i = 0; i < 3; i++ ) {
		glPushMatrix();

		for( int j = 2; j >= 0; j-- ) {
			drawQuad(i, j, textureArray, matrix);
			glTranslatef( 0, twoThirds, 0 );
		}

		glPopMatrix();
		glTranslatef( twoThirds, 0, 0 );
	}
}

// The quads will need to temporarily rotate the perspective to draw themselves at their
// respective angles, so the current matrix must be given to them.
void Face::drawQuad(int col, int row, GLuint *textureArray, GLfloat (&matrix)[16]) {
	glPushMatrix();

	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	quadsOnFace[col][row]->drawSelf(textureArray, matrix);
	//glLoadMatrixf(matrix);
	
	glPopMatrix();
}

void Face::setQuad(Quad *aQuad, int col, int row) {
	quadsOnFace[col][row] = aQuad;
}

Quad * Face::getQuad(int col, int row) {
	return quadsOnFace[col][row];
}
