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

	// for top, save bottom row in own variables and manually set
	Quad *topFaceBottomRow[3][3];
	for( int i = 0; i < 3; i++ ) {
		for( int j = 0; j < 3; j++ ) {
			//topFaceBottomRow[i][2] = adjFace.top->getQuad(i, 2); // without loop (only bottom row)
			topFaceBottomRow[i][j] = adjFace.top->getQuad(i, j);	
		}
	}

	for( int i = 0; i < 3; i++ ) { // CW: Top = Left; CCW: Top = Right
		int col;
		if( clockwise ) {
			col = 2;
			adjFace.top->setQuad( adjFace.left->getQuad(col, i), i, 2-col );
		} else {
			col = 0;
			adjFace.top->setQuad( adjFace.right->getQuad(col, i), 2-i, col );
		}
	}
	
	for( int i = 0; i < 3; i++ ) { // CW: Left = Bottom; CCW: Right = Bottom
		int row;
		if( clockwise ) {
			row = 2;
			adjFace.left->setQuad( adjFace.bottom->getQuad(i, row), row, 2-i );
		} else {
			row = 0;
			adjFace.right->setQuad( adjFace.bottom->getQuad(i, row), 2-row, i );
		}
	}

	for( int i = 0; i < 3; i++ ) { // CW: Bottom = Right; CCW: Bottom = Left
		int col;
		if( clockwise ) {
			col = 0;
			adjFace.bottom->setQuad( adjFace.right->getQuad(col, i), i, 2-col );
		} else {
			col = 2;
			adjFace.bottom->setQuad( adjFace.left->getQuad(col, i), 2-i, col );
		}
	}
	
	for( int i = 0; i < 3; i++ ) { // CW: Right = Top (Saved); CCW: Left = Top (Saved)
		int row;
		if( clockwise ) {
			row = 0;
			adjFace.right->setQuad( topFaceBottomRow[i][row], row, 2-i );
		} else {
			row = 2;
			adjFace.left->setQuad( topFaceBottomRow[i][row], 2-row, i );
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

void Face::setQuad(Quad *aQuad, int col, int row) {
	quadsOnFace[col][row] = aQuad;
}

Quad * Face::getQuad(int col, int row) {
	return quadsOnFace[col][row];
}
