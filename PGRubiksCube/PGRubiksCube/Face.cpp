/*
* Name:			Desiree Trepanier
* Project:		PG2-4 - Rubik's Cube
* Description:	The faces of the cube. A face knows which quads are on it, its adjacent faces and
				how its rotation affects those faces, and how to draw itself.
*/
#include "Face.h"

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

	setAnimationAndReverseInAdjFaces();
	setAffectedInAdjFaces();
	setRotationInformation();
}

Face::~Face() {
	delete[] &adjFaces;
	delete[] &quadsOnFace;
}

// Allows the face to know where to rotate to get to itself as well as how to handle adjacent quad rotation.
// Set the face's rotation angle, whether its on the x or y-axis, and how much the quads on the adjacent faces
// will be rotated when this face rotates. 
void Face::setRotationInformation() {
	if( faceNum == 0 || faceNum == 2 ) rotationAmt = -90.0;
	else if( faceNum == 1 || faceNum == 4 ) rotationAmt = 90.0;
	else if( faceNum == 3 ) rotationAmt = 0.0;
	else rotationAmt = 180.0;

	if( faceNum == 0 || faceNum == 1 ) {
		rotateAlongX = true;
		rotateAlongY = false;
	} else {
		rotateAlongX = false;
		rotateAlongY = true;
	}
	
	rotateQuadsAngle = (faceNum == 3) ? -90 :
					   (faceNum == 5) ? 90 :
					   (faceNum == 2 || faceNum == 4) ? 0 : 0;
}

// When rotating, the face will need to know how its rotation will affect its adjacent 
// faces as each face will affects its adjacent faces differently.
void Face::setAffectedInAdjFaces() {
	adjFaces[TOP]->affectsCol = adjFaces[BOTTOM]->affectsCol = (faceNum == 2 || faceNum == 4) ? true : false;
	adjFaces[TOP]->colOrRowAffected		= (faceNum == 0 || faceNum == 2 || faceNum == 5) ? 0 : 2;
	adjFaces[BOTTOM]->colOrRowAffected	= (faceNum == 0 || faceNum == 2 || faceNum == 3) ? 0 : 2;
	
	adjFaces[LEFT]->affectsCol = adjFaces[RIGHT]->affectsCol = (faceNum != 0 && faceNum != 1) ? true : false;
	adjFaces[LEFT]->colOrRowAffected	= (faceNum == 0) ? 0 : 2;
	adjFaces[RIGHT]->colOrRowAffected	= (faceNum != 1) ? 0 : 2;
}


// Set the animation direction and whether to reverse which pointers are moved where when rotating the adjacent
// faces about this face. Animation direction determins if the adjacent faces move in the opposite direction
// from this face's animimation angle, enabling it to move with the rotating face.
void Face::setAnimationAndReverseInAdjFaces() {
	adjFaces[TOP]->animPositiveDir		= (faceNum == 0 || faceNum == 4 || faceNum == 5) ? true : false;
	adjFaces[TOP]->reverseMove			= (faceNum == 4 || faceNum == 5) ? true : false;
	
	adjFaces[BOTTOM]->animPositiveDir	= (faceNum == 0 || faceNum == 3 || faceNum == 4) ? true : false;
	adjFaces[BOTTOM]->reverseMove		= (faceNum == 2 || faceNum == 5) ? true : false;
	
	adjFaces[LEFT]->animPositiveDir		= (faceNum != 1) ? true : false;
	adjFaces[LEFT]->reverseMove			= (faceNum == 2 || faceNum == 3) ? true : false;
	
	adjFaces[RIGHT]->animPositiveDir	= (faceNum == 0) ? true : false;
	adjFaces[RIGHT]->reverseMove		= (faceNum == 3 || faceNum == 4) ? true : false;
}

// Each face will need to know which faces are adjacent to it when the cube is rotate is rotated about the face.
void Face::setAdjFaces(Face *aTop, Face *aBottom, Face *aLeft, Face *aRight) {
	adjFaces[TOP]->face		= aTop;
	adjFaces[BOTTOM]->face	= aBottom;
	adjFaces[LEFT]->face	= aLeft;
	adjFaces[RIGHT]->face	= aRight;
}

// Determine if a face is in the middle of an animation by asking a quad on the face
// if it is still animating.
bool Face::isAnimating() {
	return (quadsOnFace[1][1]->isAnimating());
}

// Rotate to the given face in order to draw the quads.
void Face::rotateToSelf(GLfloat (&matrix)[16]) {
	glLoadMatrixf(matrix);
	glRotatef( rotationAmt, (float) rotateAlongX, (float) rotateAlongY, 0 );
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}

// Draw the underlying black quad and then the quads currently on this face.
void Face::drawSelf(GLuint *textureArray, GLfloat (&matrix)[16]) {
	glLoadMatrixf(matrix);

	glEnd();

	drawQuadsOnFace(textureArray, matrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}

// The face tells each of the quads currently on it to draw themselves.
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

// Rotates the face and all quads touching the face on adjacent faces.
// The quads on the face will rotate as well as move positions. Clockwise, (x, y) becomes (2-y, x)
// and quads are rotated -90 degrees. CCW, (x, y) becomes (y, 2-x) and quads are rotated 90 degrees.
// The quads on adjacent faces will move positions and move to another face. They may need to rotate
// to keep their orientation on the new face.
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

// Moves a column or row of one face to another, rotated about a face.
// The face that is being rotated about knows which columns or rows of its adjacent faces
// are affected when it rotates.
// The top adjacent face will always be overwritten when rotating about this face, thus needs
// a copy of its quads passed in order to set the destination face to its proper quads.
void Face::rotateQuadsAbout(AdjFace *destFace, AdjFace *srcFace, Quad *srcQuads[3][3], bool clockwise) {
	int rotation = getQuadsRotation(clockwise, srcFace, destFace);
	int destAnimAngle = getAnimAngle(destFace);
	int destAnimAxis = destFace->affectsCol ? X : Y;
	bool reverseMove = isReverseMove(clockwise, srcFace, destFace);

	for( int i = 0; i < 3; i++ ) {	
		int destCol = destFace->affectsCol	? destFace->colOrRowAffected : (reverseMove ? 2 - i : i),
			destRow = destFace->affectsCol	? (reverseMove ? 2 - i : i) : destFace->colOrRowAffected,
			srcCol	= srcFace->affectsCol	? srcFace->colOrRowAffected	: i,
			srcRow	= srcFace->affectsCol	? i	: srcFace->colOrRowAffected;

		if( srcFace != adjFaces[TOP] ) {
			destFace->face->setQuad( srcFace->face->getQuad( srcCol, srcRow ), destCol, destRow, rotation, destAnimAngle, destAnimAxis );
		} else {
			destFace->face->setQuad( srcQuads[srcCol][srcRow], destCol, destRow, rotation, destAnimAngle, destAnimAxis );
		}
	}
}

// The left and right faces on the cube need to rotate the quads 180 degrees when one of the adjacent faces affected
// by their rotation is the back face of the cube. This is because the back face is "upside down" compared to the
// other three adjacent faces.
int Face::getQuadsRotation(bool clockwise, AdjFace *srcFace, AdjFace *destFace) {
	for( int i = 0; i < 4; i++ ) {
		if( (srcFace->affectsCol && destFace->affectsCol) && (srcFace->face->isBackFace() || destFace->face->isBackFace()) ) {
			return 180;
		}
	}
	return (clockwise ? rotateQuadsAngle : -1 * rotateQuadsAngle);
}

// The animation angle will be opposite of the rotating face's if the destination face has a negative
// animation direction.
int Face::getAnimAngle(AdjFace *destFace) {
	for( int i = 0; i < 4; i++ ) {
		if( destFace == adjFaces[i] && !adjFaces[i]->animPositiveDir ) {
			return (-1 * animAngle);
		}
	}
	return animAngle;
}

// The location that the quads from the source face are moved to the destination face is reversed if
// the source face (on clockwise) or destination face (on CCW) has been set to do so.
bool Face::isReverseMove(bool clockwise, AdjFace *srcFace, AdjFace *destFace) {
	for( int i = 0; i < 4; i++ ) {
		if( (clockwise && srcFace == adjFaces[i] && adjFaces[i]->reverseMove) 
			|| (!clockwise && destFace == adjFaces[i] && adjFaces[i]->reverseMove) ) {
				return true;
		}
	}
	return false;
}

// Set a location to a given quad.
// Only called when rotating about a face. Thus, an animation will always occur following.
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

bool Face::isBackFace() {
	return (faceNum == 5);
}
