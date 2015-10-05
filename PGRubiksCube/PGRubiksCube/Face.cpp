#include "Face.h"

static double twoThirds = 2.0/3.0;

Face::Face(int textureNum) {
	for(int i = 0; i < 3; i ++) {
		for(int j = 0; j < 3; j++) {
			quadsOnFace[i][j] = new Quad(textureNum, i, j);
		}
	}

	switch(textureNum) {
		case 0: // Top
			setRotation(90.0, true);
			break;
		case 1: // Bottom
			setRotation(-90.0, true);
			break;
		case 2: // Left
			setRotation(-90.0, false);
			break;
		case 3: // Front
			setRotation(0.0, false);
			break;
		case 4: // Back
			setRotation(90.0, false);
			break;
		case 5: // Right
			setRotation(180.0, false);
			break;
	}
}

Face::~Face() {
	delete[] &quadsOnFace;
}

void Face::setAdjFaces(Face *aTop, Face *aBottom, Face *aLeft, Face *aRight) {
	adjFace.top = aTop;
	adjFace.bottom = aBottom;
	adjFace.left = aLeft;
	adjFace.right = aRight;
}

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

// Rotate to the given face in order to draw the quads.
void Face::rotateToSelf(GLfloat (&matrix)[16]) {
	glLoadMatrixf(matrix);
	glRotatef( rotationAmt, (float) rotateAlongX, (float) rotateAlongY, 0 );
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}

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
}

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

void Face::drawQuad(int col, int row, GLuint *textureArray, GLfloat (&matrix)[16]) {
	glPushMatrix();

	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	quadsOnFace[col][row]->drawSelf(textureArray, matrix);
	glLoadMatrixf(matrix);
	
	glPopMatrix();
}

Face *Face::getAdjFaceTop() {
	return adjFace.top;
}

Face *Face::getAdjFaceBottom() {
	return adjFace.bottom;
}

Face *Face::getAdjFaceLeft() {
	return adjFace.left;
}

Face *Face::getAdjFaceRight() {
	return adjFace.right;
}
