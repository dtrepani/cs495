/*
* Name:			Desiree Trepanier
* Project:		PG2-4 - Rubik's Cube
* Description:	The faces of the cube. A face knows which quads are on it, its adjacent faces and
				how its rotation affects those faces, and how to draw itself.
*/
#include "Face.h"

// TODO: Fix rotating quads on top and bottom faces when rotating about face

Face::Face(int aFaceNum) {
	animAngle = 0;
	faceNum = aFaceNum;

	for( int i = 0; i < 3; i ++ ) {
		for( int j = 0; j < 3; j++ ) {
			quadsOnFace[i][j] = new Quad(faceNum, i, j);
		}
	}

	for( int i = 0; i < 4; i++ ) {
		adjFaces[i] = new AdjFace;
	}

	// Determines which adjacent faces move in the opposite direction from the given animAngle in
	// order to move with the rotating face
	bool animPositiveDirs[4] = {
		(faceNum == 0 || faceNum == 4 || faceNum == 5) ? true : false,
		(faceNum == 0 || faceNum == 3 || faceNum == 4) ? true : false,
		(faceNum != 1) ? true : false,
		(faceNum == 0) ? true : false
	};

	switch(faceNum) {
		case 0: // Top
			setFaceInformation(-90.0, true, false);
			setAffectedInAdjFaces(false, 0, 0, false, 0, 0, animPositiveDirs);
			break;
		case 1: // Bottom
			setFaceInformation(90.0, true, false);
			setAffectedInAdjFaces(false, 2, 2, false, 2, 2, animPositiveDirs);
			break;
		case 2: // Left
			setFaceInformation(-90.0, false, true);
			setAffectedInAdjFaces(true, 0, 0, true, 2, 0, animPositiveDirs);
			break;
		case 3: // Front
			setFaceInformation(0.0, false, true);
			setAffectedInAdjFaces(false, 2, 0, true, 2, 0, animPositiveDirs);
			break;
		case 4: // Right
			setFaceInformation(90.0, false, true);
			setAffectedInAdjFaces(true, 2, 2, true, 2, 0, animPositiveDirs);
			break;
		case 5: // Back
			setFaceInformation(180.0, false, true);
			setAffectedInAdjFaces(false, 0, 2, true, 2, 0, animPositiveDirs);
			break;
	}
}

Face::~Face() {
	delete[] &adjFaces;
	delete[] &quadsOnFace;
}

bool Face::isAnimating() {
	return (quadsOnFace[1][1]->isAnimating());
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
	animAngle = clockwise ? 90 : -90;
	Quad *quadsBeforeRotation[3][3];
	memcpy(&quadsBeforeRotation[0][0], &quadsOnFace[0][0], sizeof(quadsBeforeRotation[0][0]) * 9);
	
	angle = ( angle + rotation ) % 360;

	for( int i = 0; i < 3; i++ ) {
		for( int j = 0; j < 3; j++ ) {
			quadsOnFace[i][j]->addToAngle(rotation, animAngle, Z);
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

	glEnd(); // TODO: Why won't cube draw properly with this gone?

	drawQuadsOnFace(textureArray, matrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}



// The face draws each of the quads currently on it.
// The quads do not know where on the face they are, therefore the face must tell them.
void Face::drawQuadsOnFace(GLuint *textureArray, GLfloat (&matrix)[16]) {
	double twoThirds = 2.0/3.0;

	for( int i = 0; i < 3; i++ ) {
		for( int j = 0; j < 3; j++ ) {
			drawQuad(2-i, j, twoThirds - i*twoThirds, twoThirds - j*twoThirds, textureArray, matrix);
		}
	}
}

// The quads will need to temporarily rotate the perspective to draw themselves at their
// respective angles, so the current matrix must be given to them.
void Face::drawQuad(int col, int row, double colLoc, double rowLoc, GLuint *textureArray, GLfloat (&matrix)[16]) {
	glPushMatrix();
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	quadsOnFace[col][row]->drawSelf(colLoc, rowLoc, textureArray, matrix);
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
// The bottom face of the cube's adjacent faces will rotate opposite of what the other faces would.
void Face::rotateQuadsAboutOrder(bool clockwise) {
	if(isBottomFace()) clockwise = !clockwise;

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

// TODO: Fix comment
// Rotates one side of a face to another side.
// The face that is being rotated about knows which columns or rows of its adjacent faces
// are affected when it rotates. The top adjacent face will always be overwritten when
// rotating about this face, thus needs a copy of its quads passed in order to set the destination
// face to its proper quads.
void Face::rotateQuadsAbout(AdjFace *destFace, AdjFace *srcFace, Quad *srcQuads[3][3], bool clockwise) {
	int rotation = rotateQuads ? (clockwise ? -90 : 90) : 0;
	int destAnimAngle = animAngle;
	int destAnimAxis = destFace->affectsCol ? X : Y;
	bool topOrBottomFace = isTopOrBottomFace(clockwise ? destFace : srcFace);

	for( int i = 0; i < 4; i++ ) {
		if( destFace == adjFaces[i] && !adjFaces[i]->animPositiveDir ) {
			destAnimAngle = -1 * animAngle;
			break;
		}
	}

	for( int i = 0; i < 3; i++ ) {	
		int destCol = destFace->affectsCol	? destFace->colOrRowAffected : (topOrBottomFace ? 2 - i : i),
			destRow = destFace->affectsCol	? (topOrBottomFace ? 2 - i : i) : destFace->colOrRowAffected,
			srcCol	= srcFace->affectsCol	? srcFace->colOrRowAffected	: i,
			srcRow	= srcFace->affectsCol	? i	: srcFace->colOrRowAffected;

		if( srcFace != adjFaces[TOP] ) {
			destFace->face->setQuad( srcFace->face->getQuad( srcCol, srcRow ), destCol, destRow, rotation, destAnimAngle, destAnimAxis );
		} else {
			destFace->face->setQuad( srcQuads[srcCol][srcRow], destCol, destRow, rotation, destAnimAngle, destAnimAxis );
		}
	}
}

// Various information dependent on which face the current one is that is necessary for the face to function.
// rotateQuads determines if the quads on adjacent faces will be rotated when this faces rotates. The top and
// bottom faces of the cube do not rotate said quads.
void Face::setFaceInformation(float aRotationAmt, bool aRotateAlongX, bool aRotateQuads) {
	setRotation(aRotationAmt, aRotateAlongX);
	rotateQuads = aRotateQuads;
}

// When rotating, the face will need to know how its rotation will affect its adjacent 
// faces as each face will affects its adjacent faces differently.
void Face::setAffectedInAdjFaces(bool topAndBottomAffectsCol,	int topColOrRowAffected,	int bottomColOrRowAffected,
								 bool leftAndRightAffectsCol,	int leftColOrRowAffected,	int rightColOrRowAffected,
								 bool animPositiveDirs[]) {
	adjFaces[TOP]->affectsCol	  = topAndBottomAffectsCol;		adjFaces[TOP]->colOrRowAffected	= topColOrRowAffected;
	adjFaces[BOTTOM]->affectsCol  = topAndBottomAffectsCol;		adjFaces[BOTTOM]->colOrRowAffected	= bottomColOrRowAffected;
	adjFaces[LEFT]->affectsCol	  = leftAndRightAffectsCol;		adjFaces[LEFT]->colOrRowAffected	= leftColOrRowAffected;
	adjFaces[RIGHT]->affectsCol   = leftAndRightAffectsCol;		adjFaces[RIGHT]->colOrRowAffected	= rightColOrRowAffected;
	
	adjFaces[TOP]->animPositiveDir		= animPositiveDirs[TOP];
	adjFaces[BOTTOM]->animPositiveDir	= animPositiveDirs[BOTTOM];
	adjFaces[LEFT]->animPositiveDir		= animPositiveDirs[LEFT];
	adjFaces[RIGHT]->animPositiveDir	= animPositiveDirs[RIGHT];
}

void Face::setQuad(Quad *aQuad, int col, int row, int angle, int animAngle, int animAxis) {
	quadsOnFace[col][row] = aQuad;
	quadsOnFace[col][row]->addToAngle(angle, animAngle, animAxis);
}

Quad * Face::getQuad(int col, int row) {
	return quadsOnFace[col][row];
}

bool Face::isBottomFace() {
	return (faceNum == 1);
}

bool Face::isTopFace() {
	return (faceNum == 0);
}

bool Face::isTopOrBottomFace(AdjFace *aFace) {
	return (aFace->face->isBottomFace() || aFace->face->isTopFace());
}
