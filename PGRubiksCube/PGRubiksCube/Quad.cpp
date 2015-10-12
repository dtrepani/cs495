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
}

Quad::~Quad() {
}

// The quad may need to be rotated as the faces rotate on the cube.
void Quad::addToAngle(int angleToBeAdded) {
	angle = ( angle + angleToBeAdded ) % 360;
}

// Draw the quad. The matrix given should already be setup for the quad to draw itself. The quad
// knows what texture and what part of the texture to draw on itself as well to draw itself slightly
// off the face to avoid clipping.
void Quad::drawSelf(GLuint *textureArray, GLfloat (&matrix)[16]) {
	double	oneThird = 0.325,
			negOneThird = -0.325,
			zWithOffset = 1.005,
			colX = texCol/3.0,
			rowY = texRow/3.0;
	
	glLoadMatrixf(matrix);

	glBindTexture( GL_TEXTURE_2D, textureArray[texture] );
	glRotatef( (float)angle, 0, 0, 1 );
			
	glBegin(GL_QUADS);
		glTexCoord2f( colX,				rowY + oneThird );	glVertex3f( negOneThird,	negOneThird,	zWithOffset );
		glTexCoord2f( colX + oneThird,	rowY + oneThird );	glVertex3f( oneThird,		negOneThird,	zWithOffset );
		glTexCoord2f( colX + oneThird,	rowY			);	glVertex3f( oneThird,		oneThird,		zWithOffset );
		glTexCoord2f( colX,				rowY			);	glVertex3f( negOneThird,	oneThird,		zWithOffset );
	glEnd();
}
