#include "Quad.h"

int texture,
	texCol,
	texRow,
	angle;

Quad::Quad(int aTexture, int aTexCol, int aTexRow, int anAngle) {
	texture = aTexture;
	texCol = aTexCol;
	texRow = aTexRow;
	angle = anAngle;
}

Quad::~Quad() {
}

void Quad::addToAngle(int angleToBeAdded) {
	angle = ( angle + angleToBeAdded ) % 360;
}

void Quad::drawSelf(GLuint *textureArray) {
	double	oneThird = 1.0/3.0,
			negOneThird = -1.0/3.0,
			zWithOffset = 1.005,
			colX = texCol/3.0,
			rowY = texRow/3.0;

	//cout << "Texture is " << texture << ", at (" << texCol << ", " << texRow << ")" << endl;
	//glBindTexture( GL_TEXTURE_2D, textureArray[texture] );
	glRotatef( (float)angle, 0, 0, 1 );
			
	glBegin(GL_QUADS);
		glTexCoord2f( colX,				rowY + oneThird );	glVertex3f( negOneThird,	negOneThird,	zWithOffset );
		glTexCoord2f( colX + oneThird,	rowY + oneThird );	glVertex3f( oneThird,		negOneThird,	zWithOffset );
		glTexCoord2f( colX + oneThird,	rowY			);	glVertex3f( oneThird,		oneThird,		zWithOffset );
		glTexCoord2f( colX,				rowY			);	glVertex3f( negOneThird,	oneThird,		zWithOffset );
	glEnd();
}


int Quad::getTexture() {
	return texture;
}

int Quad::getTextureColumn() {
	return texCol;
}

int Quad::getTextureRow() {
	return texRow;
}

int Quad::getAngle() {
	return angle;
}

void Quad::setTexture(int aTexture) {
	texture = aTexture;
}

void Quad::setTextureColumn(int aTexCol) {
	texCol = aTexCol;
}

void Quad::setTextureRow(int aTexRow) {
	texRow = aTexRow;
}

void Quad::setAngle(int anAngle) {
	angle = anAngle;
}
