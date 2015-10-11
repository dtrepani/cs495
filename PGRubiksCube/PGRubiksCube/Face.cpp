/*
* Name:			Desiree Trepanier
* Project:		PG2-4 - Rubik's Cube
* Description:
*/
#include "Face.h"

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TODO:
	- Fix orientation of quads when rotated to different face
	- Quads mixing up when multiple rotations occur?
*/

static double twoThirds = 2.0/3.0;

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
			setAffectedInAdjFaces(true, 0, 0, true, 2, 0);
			break;
		case 3: // Front
			setRotation(0.0, false);
			setAffectedInAdjFaces(false, 2, 0, true, 2, 0);
			break;
		case 4: // Right
			setRotation(90.0, false);
			setAffectedInAdjFaces(true, 2, 2, true, 2, 0);
			break;
		case 5: // Back
			setRotation(180.0, false);
			setAffectedInAdjFaces(false, 0, 2, true, 2, 0);
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

	if(clockwise) {
		rotateQuadsAbout(adjTop,	adjLeft,	NULL);
		rotateQuadsAbout(adjLeft,	adjBottom,	NULL);
		rotateQuadsAbout(adjBottom, adjRight,	NULL);
		rotateQuadsAbout(adjRight,	adjTop,		topFaceBottomRow);
	} else {
		rotateQuadsAbout(adjTop,	adjRight,	NULL);
		rotateQuadsAbout(adjRight,	adjBottom,	NULL);
		rotateQuadsAbout(adjBottom, adjLeft,	NULL);
		rotateQuadsAbout(adjLeft,	adjTop,		topFaceBottomRow);
	}
}


// Rotates one side of a face to another side.
// The face that is being rotated about knows which columns or rows of its adjacent faces
// are affected when it rotates. The top adjacent face will always be overwritten when
// rotating about this face, thus needs a copy of its quads passed in order to set the destination
// face to its proper quads.
void Face::rotateQuadsAbout(AdjFace *destFace, AdjFace *srcFace, Quad *srcQuads[3][3]) {
	for( int i = 0; i < 3; i++ ) {
		int srcCol, srcRow, destCol, destRow;

		/*
		int destCol = destFace->affectsCol ? destFace->colOrRowAffected : i,
			destRow = destFace->affectsCol ? i : destFace->colOrRowAffected,
			srcCol	= srcFace->affectsCol ? srcFace->colOrRowAffected : i,
			srcRow	= srcFace->affectsCol ? i : srcFace->colOrRowAffected;
			*/

		if( srcFace->affectsCol ) {
			srcCol = srcFace->colOrRowAffected;
			srcRow = i;
		} else {
			srcCol = i;
			srcRow = srcFace->colOrRowAffected;
		}

		if( destFace->affectsCol ) {
			destCol = destFace->colOrRowAffected;
			destRow = i;
		} else {
			destCol = i;
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
