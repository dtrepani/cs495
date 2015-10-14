/*
* Name:			Desiree Trepanier
* Project:		PG2-4 - Rubik's Cube
* Description:	The faces of the cube. A face knows which quads are on it, its adjacent faces and
				how its rotation affects those faces, and how to draw itself.
*/
#include "Face.h"

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TODO:
	- Fix orientation of quads when rotated to different face
	- Quads mixing up when multiple rotations occur?
*/

Face::Face(int faceNum) {
	for( int i = 0; i < 3; i ++ ) {
		for( int j = 0; j < 3; j++ ) {
			quadsOnFace[i][j] = new Quad(faceNum, i, j);
		}
	}

	for( int i = 0; i < 4; i++ ) {
		adjFaces[i] = new AdjFace;
	}

	switch(faceNum) {
		case 0: // Top
			setRotation(-90.0, true);
			setAffectedInAdjFaces(false, 0, 0, false, 0, 0);
			rotateQuads = false;
			break;
		case 1: // Bottom
			setRotation(90.0, true);
			setAffectedInAdjFaces(false, 2, 2, false, 2, 2);
			rotateQuads = false;
			break;
		case 2: // Left
			setRotation(-90.0, false);
			setAffectedInAdjFaces(true, 0, 0, true, 2, 0);
			rotateQuads = true;
			break;
		case 3: // Front
			setRotation(0.0, false);
			setAffectedInAdjFaces(false, 2, 0, true, 2, 0);
			rotateQuads = true;
			break;
		case 4: // Right
			setRotation(90.0, false);
			setAffectedInAdjFaces(true, 2, 2, true, 2, 0);
			rotateQuads = true;
			break;
		case 5: // Back
			setRotation(180.0, false);
			setAffectedInAdjFaces(false, 0, 2, true, 2, 0);
			rotateQuads = true;
			break;
	}
}

Face::~Face() {
	delete[] &adjFaces;
	delete[] &quadsOnFace;
}

// Each face will need to know which faces are adjacent to it when the cube is rotate is rotated about the face.
void Face::setAdjFaces(Face *aTop, Face *aBottom, Face *aLeft, Face *aRight) {
	adjFaces[TOP]->face	= aTop;
	adjFaces[BOTTOM]->face = aBottom;
	adjFaces[LEFT]->face	= aLeft;
	adjFaces[RIGHT]->face	= aRight;
}

// Rotate to the given face in order to draw the quads.
void Face::rotateToSelf(GLfloat (&matrix)[16]) {
	glLoadMatrixf(matrix);
	glRotatef( rotationAmt, (float) rotateAlongX, (float) rotateAlongY, 0 );
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}

// Rotates the face and all quads touching the face on adjacent faces.
// The quads on the face will rotate as well as move positions.
// The quads on adjacent faces will move positions and move to another face. 
// They may need to rotate to keep their orientation on the new face.
void Face::rotateAbout(bool clockwise) {
	int rotation = clockwise ? -90 : 90;
	Quad *quadsBeforeRotation[3][3];
	memcpy(&quadsBeforeRotation[0][0], &quadsOnFace[0][0], sizeof(quadsBeforeRotation[0][0]) * 9);
	
	angle = ( angle + rotation ) % 360;

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

	rotateQuadsAboutOrder(clockwise);
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



// The face draws each of the quads currently on it.
// The quads do not know how to get to themselves to draw themselves, so translation
// on the face level is necessary.
void Face::drawQuadsOnFace(GLuint *textureArray, GLfloat (&matrix)[16]) {
	double twoThirds = 2.0/3.0;

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
	glPopMatrix();
}

// Set the face's rotation angle, whether its on the x or y-axis.
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

// Depending on if the face is rotating clockwise or counterclockwise, the left or right adjacent
// face will need to switch places in rotation order. The top adjacent face will always be overwritten
// so a copy of its quads must be made.
void Face::rotateQuadsAboutOrder(bool clockwise) {
	AdjFace *firstAdjFace  = clockwise ? adjFaces[LEFT]  : adjFaces[RIGHT];
	AdjFace *secondAdjFace = clockwise ? adjFaces[RIGHT] : adjFaces[LEFT];
	Quad *topAdjFace[3][3];

	for( int i = 0; i < 3; i++ ) {
		for( int j = 0; j < 3; j++ ) {
			topAdjFace[i][j] = adjFaces[TOP]->face->getQuad(i, j);
		}
	}

	rotateQuadsAbout(adjFaces[TOP],		firstAdjFace,		NULL,		clockwise);
	rotateQuadsAbout(firstAdjFace,		adjFaces[BOTTOM],	NULL,		clockwise);
	rotateQuadsAbout(adjFaces[BOTTOM],	secondAdjFace,		NULL,		clockwise);
	rotateQuadsAbout(secondAdjFace,		adjFaces[TOP],		topAdjFace,	clockwise);
}

// Rotates one side of a face to another side.
// The face that is being rotated about knows which columns or rows of its adjacent faces
// are affected when it rotates. The top adjacent face will always be overwritten when
// rotating about this face, thus needs a copy of its quads passed in order to set the destination
// face to its proper quads.
void Face::rotateQuadsAbout(AdjFace *destFace, AdjFace *srcFace, Quad *srcQuads[3][3], bool clockwise) {
	int rotation = rotateQuads ? (clockwise ? -90 : 90) : 0;

	// TODO:	Reverse copy order from bottom face for front and back face (2 maps to 0 rather than 0 maps to 0)
	//			Quad rotation also reversed for all faces with src Bottom (90 == -90 and vice versa))
	// IDEA:	Add reverse bool to AdjFace

	for( int i = 0; i < 3; i++ ) {	
		int index	= (srcFace == adjFaces[BOTTOM] /*&& (this == back || this == front)*/ ) ? 2 - i : i, // NOT WORKING
			destCol = destFace->affectsCol	? destFace->colOrRowAffected	: index,
			destRow = destFace->affectsCol	? index							: destFace->colOrRowAffected,
			srcCol	= srcFace->affectsCol	? srcFace->colOrRowAffected		: index,
			srcRow	= srcFace->affectsCol	? index							: srcFace->colOrRowAffected;

		if( srcFace != adjFaces[TOP] ) {
			destFace->face->setQuad( srcFace->face->getQuad( srcCol, srcRow ), destCol, destRow, rotation );
		} else {
			destFace->face->setQuad( srcQuads[srcCol][srcRow], destCol, destRow, rotation );
		}
	}
}

// When rotating, the face will need to know how its rotation will affect its adjacent 
// faces as each face will affects its adjacent faces differently.
void Face::setAffectedInAdjFaces(bool topAndBottomAffectsCol,	int topColOrRowAffected,	int bottomColOrRowAffected,
								 bool leftAndRightAffectsCol,	int leftColOrRowAffected,	int rightColOrRowAffected) {
	adjFaces[TOP]->affectsCol	  = topAndBottomAffectsCol;		adjFaces[TOP]->colOrRowAffected	= topColOrRowAffected;
	adjFaces[BOTTOM]->affectsCol = topAndBottomAffectsCol;		adjFaces[BOTTOM]->colOrRowAffected	= bottomColOrRowAffected;
	adjFaces[LEFT]->affectsCol	  = leftAndRightAffectsCol;		adjFaces[LEFT]->colOrRowAffected	= leftColOrRowAffected;
	adjFaces[RIGHT]->affectsCol  = leftAndRightAffectsCol;		adjFaces[RIGHT]->colOrRowAffected	= rightColOrRowAffected;
}

											// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TODO: setQuad should adjust the quad to the orientation of the new face
void Face::setQuad(Quad *aQuad, int col, int row, int angle) {
	quadsOnFace[col][row] = aQuad;
	quadsOnFace[col][row]->addToAngle(angle);
}

Quad * Face::getQuad(int col, int row) {
	return quadsOnFace[col][row];
}

int Face::getAngle() {
	return angle;
}
