/*
* Name:			Desiree Trepanier
* Project:		PG2-4 - Rubik's Cube
* Description:	The quads on the cube. A quad knows its texture, its position on the texture, the
				angle its drawn at, and how to draw itself.
*/
#include "Quad.h"

Quad::Quad(int aTexture, int aTexCol, int aTexRow) {
	texture = aTexture;
	texCol = aTexCol;
	texRow = aTexRow;
	angle = 0;
	animAngle = 0;
	animX = animY = animZ = false;
}

Quad::~Quad() {
}

bool Quad::isAnimating() {
	return (animAngle != 0);
}

// The quad may need to be rotated as the faces rotate on the cube.
void Quad::addToAngle(int angleToBeAdded, int anAnimAngle, int anAnimAxis) {
	angle = ( angle + angleToBeAdded ) % 360;

	animAngle = anAnimAngle;
	switch(anAnimAxis) {
	case 0:
		setAnimAxis(true, false, false);
		break;
	case 1:
		setAnimAxis(false, true, false);
		break;
	case 2:
		setAnimAxis(false, false, true);
	}
}

// Draw the quad. The matrix given should already be setup for the quad to draw itself. The quad
// knows what texture and what part of the texture to draw on itself as well to draw itself slightly
// off the face to avoid clipping.
void Quad::drawSelf(double colLoc, double rowLoc, GLuint *textureArray, GLfloat (&matrix)[16]) {
	
	glLoadMatrixf(matrix);

	if( animAngle != 0 ) {
		glRotatef( (float)animAngle, animX, animY, animZ );
		animAngle = (animAngle < 0) ? animAngle + 2 : animAngle - 2;
	}

	glTranslatef(colLoc, rowLoc, 0);
	glRotatef( (float)angle, 0, 0, 1 );

	glColor3f(0, 0, 0);
	drawGLQuad(1.0, false);
	glColor3f(1, 1, 1);
	glBindTexture( GL_TEXTURE_2D, textureArray[texture] );
	drawGLQuad(1.005, true);
}


// The Quad is responsible for drawing both the black quad beneath it and the slightly smaller, textured quad.
void Quad::drawGLQuad(double zOffset, bool drawTex) {
	double	oneThird	= drawTex ? 0.325  : 1.0/3.0,
			negOneThird = drawTex ? -0.325 : -1.0/3.0,
			colX = texCol/3.0,
			rowY = texRow/3.0;

	glBegin(GL_QUADS);
		if(drawTex) { glTexCoord2f( colX,				rowY + oneThird ); }	glVertex3f( negOneThird,	negOneThird,	zOffset );
		if(drawTex) { glTexCoord2f( colX + oneThird,	rowY + oneThird ); }	glVertex3f( oneThird,		negOneThird,	zOffset );
		if(drawTex) { glTexCoord2f( colX + oneThird,	rowY			); }	glVertex3f( oneThird,		oneThird,		zOffset );
		if(drawTex) { glTexCoord2f( colX,				rowY			); }	glVertex3f( negOneThird,	oneThird,		zOffset );
	glEnd();
}

// Given an axis number, the Quad can determine which axis it should animate itself rotating on.
void Quad::setAnimAxis(bool x, bool y, bool z) {
	animX = x;
	animY = y;
	animZ = z;
}
