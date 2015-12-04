#include "BlockEntity.h"
#include "LinkedList.h"

BlockEntity::BlockEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, Orientation aOrientation, float widthX, float heightY, float lengthZ)
: PlaneEntity(aPosition, aTexture, aVertices, aOrientation) {
	float posX = aPosition->getX(),
		  posY = aPosition->getY(),
		  posZ = aPosition->getZ();

	planes = new LinkedList();

	widthX /= 2;
	heightY /= 2;
	lengthZ /= 2;

	GLfloat frontFace[12] = {
		 widthX, -heightY,  lengthZ,
		-widthX, -heightY,  lengthZ,
		-widthX,  heightY,  lengthZ,
		 widthX,  heightY,  lengthZ,
	};
	createAndAdd(posX, posY, posZ, &frontFace[0], VERTICAL_X);

	GLfloat backFace[12] = {
		 widthX, -heightY, -lengthZ,
		-widthX, -heightY, -lengthZ,
		-widthX,  heightY, -lengthZ,
		 widthX,  heightY, -lengthZ,
	};
	createAndAdd(posX, posY, posZ, &backFace[0], VERTICAL_X);

	GLfloat topFace[12] = {
		-widthX,  heightY,  lengthZ,
		 widthX,  heightY,  lengthZ,
		 widthX,  heightY, -lengthZ,
		-widthX,  heightY, -lengthZ,
	};
	createAndAdd(posX, posY, posZ, &topFace[0], HORIZONTAL);

	GLfloat bottomFace[12] = {
		-widthX, -heightY,  lengthZ,
		 widthX, -heightY,  lengthZ,
		 widthX, -heightY, -lengthZ,
		-widthX, -heightY, -lengthZ,
	};
	createAndAdd(posX, posY, posZ, &bottomFace[0], HORIZONTAL);

	GLfloat rightFace[12] = {
		 widthX, -heightY, -lengthZ,
		 widthX, -heightY,  lengthZ,
		 widthX,  heightY,  lengthZ,
		 widthX,  heightY, -lengthZ,
	};
	createAndAdd(posX, posY, posZ, &rightFace[0], VERTICAL_Z);

	GLfloat leftFace[12] = {
		-widthX, -heightY, -lengthZ,
		-widthX, -heightY,  lengthZ,
		-widthX,  heightY,  lengthZ,
		-widthX,  heightY, -lengthZ,
	};
	createAndAdd(posX, posY, posZ, &leftFace[0], VERTICAL_Z);
}

BlockEntity::~BlockEntity(void) {
	delete planes;
}

void BlockEntity::createAndAdd(float x, float y, float z, GLfloat* vertices, Orientation orientation) {
	planes->add(new PlaneEntity(new Vector(x, y, z), texture, &vertices[0], orientation));
}

bool BlockEntity::hasCollided(Entity* otherEntity) {
	return planes->hasCollided(otherEntity);
}

bool BlockEntity::isMovingToward(Entity* otherEntity) {
	return planes->isMovingToward(otherEntity);
}

bool BlockEntity::checkForCollision(Entity* otherEntity) {
	return planes->checkForCollision(otherEntity);
}

void BlockEntity::drawSelf() {
	planes->drawSelf();
}
