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
		-widthX,  heightY,  1.0f,
		 widthX,  heightY,  1.0f,
		 widthX, -heightY,  1.0f,
		-widthX, -heightY,  1.0f,
	};
	createAndAdd(posX, posY, posZ, frontFace, VERTICAL_X);

	GLfloat backFace[12] = {
		-widthX,  heightY, -1.0f,
		 widthX,  heightY, -1.0f,
		 widthX, -heightY, -1.0f,
		-widthX, -heightY, -1.0f,
	};
	createAndAdd(posX, posY, posZ, backFace, VERTICAL_X);

	GLfloat topFace[12] = {
		-widthX,  1.0f,  lengthZ,
		 widthX,  1.0f,  lengthZ,
		 widthX,  1.0f, -lengthZ,
		-widthX,  1.0f, -lengthZ,
	};
	createAndAdd(posX, posY, posZ, topFace, HORIZONTAL);

	GLfloat bottomFace[12] = {
		-widthX, -1.0f,  lengthZ,
		 widthX, -1.0f,  lengthZ,
		 widthX, -1.0f, -lengthZ,
		-widthX, -1.0f, -lengthZ,
	};
	createAndAdd(posX, posY, posZ, topFace, HORIZONTAL);

	GLfloat rightFace[12] = {
		 1.0f, -heightY,  lengthZ,
		 1.0f,  heightY,  lengthZ,
		 1.0f,  heightY, -lengthZ,
		 1.0f, -heightY, -lengthZ,
	};
	createAndAdd(posX, posY, posZ, rightFace, VERTICAL_Z);

	GLfloat leftFace[12] = {
		-1.0f, -heightY,  lengthZ,
		-1.0f,  heightY,  lengthZ,
		-1.0f,  heightY, -lengthZ,
		-1.0f, -heightY, -lengthZ,
	};
	createAndAdd(posX, posY, posZ, leftFace, VERTICAL_Z);
}

BlockEntity::~BlockEntity(void) {
	delete planes;
}

void BlockEntity::createAndAdd(float x, float y, float z, GLfloat* vertices, Orientation orientation) {
	planes->add(new PlaneEntity(new Vector(x, y, z), texture, vertices, orientation));
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
