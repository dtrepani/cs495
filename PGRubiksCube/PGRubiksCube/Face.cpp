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
			reverseAboutFaceRotation = false;
			break;
		case 1: // Bottom
			setRotation(90.0, true);
			reverseAboutFaceRotation = false;
			break;
		case 2: // Left
			setRotation(-90.0, false);
			reverseAboutFaceRotation = true;
			break;
		case 3: // Front
			setRotation(0.0, false);
			reverseAboutFaceRotation = false;
			break;
		case 4: // Back
			setRotation(90.0, false);
			reverseAboutFaceRotation = false;
			break;
		case 5: // Right
			setRotation(180.0, false);
			reverseAboutFaceRotation = true;
			break;
	}
}

Face::~Face() {
	delete[] &quadsOnFace;
}

// Each face will need to know which faces are adjacent to it when the cube is rotate is rotated about the face.
void Face::setAdjFaces(Face *aTop, Face *aBottom, Face *aLeft, Face *aRight) {
	adjTop = aTop;
	adjBottom = aBottom;
	adjLeft = aLeft;
	adjRight = aRight;
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
			//topFaceBottomRow[i][2] = adjTop->getQuad(i, 2); // without loop (only bottom row)
			topFaceBottomRow[i][j] = adjTop->getQuad(i, j);	
		}
	}
	
	rotateQuadsAbout(adjTop,	adjLeft,	NULL,				clockwise, true,	2);
	rotateQuadsAbout(adjLeft,	adjBottom,	NULL,				clockwise, false,	0);
	rotateQuadsAbout(adjBottom, adjRight,	NULL,				clockwise, true,	0);
	rotateQuadsAbout(adjRight,	NULL,		topFaceBottomRow,	clockwise, false,	2);
	
	/*for( int i = 0; i < 3; i++ ) { // CW: Top = Left; CCW: Top = Right
		int col;
		if( clockwise ) {
			col = 2;
			//adjTop->setQuad( adjLeft->getQuad(col, i), i, 2-col );
			adjTop->setQuad( adjLeft->getQuad(col, i), 2-i, col );
			cout << "Putting left's (" << col << ", " << i << ") in top's (" << 2-i << ", " << col << ")" << endl;
		} else {
			//col = 0;
			//adjTop->setQuad( adjFace.right->getQuad(col, i), 2-i, col );
			col = 0;
			adjTop->setQuad( adjLeft->getQuad(col, i), i, 2-col );
		}
	}
	
	for( int i = 0; i < 3; i++ ) { // CW: Left = Bottom; CCW: Right = Bottom
		int row;
		if( clockwise ) {
			row = 0;
			//adjLeft->setQuad( adjBottom->getQuad(i, row), row, 2-i );
			adjLeft->setQuad( adjBottom->getQuad(i, row), 2-row, i );
			cout << "Putting bottom's (" << i << ", " << row << ") in left's (" << 2-row << ", " << i << ")" << endl;
		} else {
			//row = 0;
			//adjRight->setQuad( adjBottom->getQuad(i, row), 2-row, i );
			row = 2;
			adjLeft->setQuad( adjBottom->getQuad(i, row), row, 2-i );
		}
	}

	for( int i = 0; i < 3; i++ ) { // CW: Bottom = Right; CCW: Bottom = Left
		int col;
		if( clockwise ) {
			col = 0;
			//adjBottom->setQuad( adjRight->getQuad(col, i), i, 2-col );
			adjBottom->setQuad( adjRight->getQuad(col, i), 2-i, col );
			cout << "Putting right's (" << col << ", " << i << ") in bottom's (" << 2-i << ", " << col << ")" << endl;
		} else {
			//col = 2;
			//adjBottom->setQuad( adjLeft->getQuad(col, i), 2-i, col );
			col = 2;
			adjBottom->setQuad( adjRight->getQuad(col, i), i, 2-col );
		}
	}
	
	for( int i = 0; i < 3; i++ ) { // CW: Right = Top (Saved); CCW: Left = Top (Saved)
		int row;
		if( clockwise ) {
			row = 2;
			adjRight->setQuad( topFaceBottomRow[i][row], 2-row, i );
			cout << "Putting top's (" << i << ", " << row << ") in right's (" << 2-row << ", " << i << ")" << endl;
		} else {
			//row = 2;
			//adjFace.left->setQuad( topFaceBottomRow[i][row], 2-row, i );
			row = 0;
			adjRight->setQuad( topFaceBottomRow[i][row], row, 2-i );
		}
	}*/
}

void Face::rotateQuadsAbout(Face *destFace, Face *srcFace, Quad *srcQuads[3][3], bool clockwise, bool colToRow, int srcCW) {
	srcCW = clockwise ? srcCW : (srcCW + 2) % 2;

	for( int i = 0; i < 3; i ++) {
		int srcCol	= colToRow	? srcCW		:	i,
			srcRow	= colToRow	? i			:	srcCW,
			destCol = clockwise ? 2-srcRow	:	srcRow,
			destRow = clockwise ? srcCol	:	2-srcCol;

		if( colToRow && reverseAboutFaceRotation ) {
			destCol = clockwise ? srcRow	: 2-srcRow;
			destRow = clockwise ? 2-srcCol	: srcCol;
		}

		if( srcFace ) {
			destFace->setQuad( srcFace->getQuad(srcCol, srcRow), destCol, destRow );
		} else {
			destFace->setQuad( srcQuads[srcCol][srcRow], destCol, destRow );
		}
		
		//cout << "Putting sources's (" << srcCol << ", " << srcRow << ") in destination's (" << destCol << ", " << destRow << ")" << endl;
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
