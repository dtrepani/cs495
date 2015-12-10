#include "BlockEntity.h"

BlockEntity::BlockEntity(Vector* aPosition, GLuint *aTexture, float widthX, float heightY, float lengthZ)
: PlaneEntity(aPosition, aTexture, NULL, BLOCK) {
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
	PlaneEntity* plane = new PlaneEntity(new Vector(0, 0, 0), texture, &vertices[0], orientation);
	plane->setParent(this);
	planes->add(plane);
}

float BlockEntity::getDist(Vector* otherPosition) {
	Vector* min = new Vector(planes->getMin(X), planes->getMin(Y), planes->getMin(Z));
	Vector* max = new Vector(planes->getMax(X), planes->getMax(Y), planes->getMax(Z));
	float dist = 0;
	
	if(otherPosition->getX() < min->getX())
		dist += (float)pow(min->getX() - otherPosition->getX(), 2);
	else if(otherPosition->getX() > max->getX())
		dist += (float)pow(otherPosition->getX() - max->getX(), 2);

	if(otherPosition->getY() < min->getY())
		dist += (float)pow(min->getY() - otherPosition->getY(), 2);
	else if(otherPosition->getY() > max->getY())
		dist += (float)pow(otherPosition->getY() - max->getY(), 2);
	
	if(otherPosition->getZ() < min->getZ())
		dist += (float)pow(min->getZ() - otherPosition->getZ(), 2);
	else if(otherPosition->getZ() > max->getZ())
		dist += (float)pow(otherPosition->getZ() - max->getZ(), 2);

	return dist;
}

bool BlockEntity::hasCollided(Entity* otherEntity) { 
	return getDist(otherEntity->getPosition()) <= (float)pow(otherEntity->getRadius(), 2);
}

bool BlockEntity::isMovingToward(Entity* otherEntity) {
	if(getDist(otherEntity->getPosition()) > getDist( (otherEntity->getPosition())->add(otherEntity->getVelocity()) )) return true;
	return false;
}

bool BlockEntity::checkForCollision(Entity* otherEntity) {
	if(hasCollided(otherEntity) && isMovingToward(otherEntity)) {
		if((otherEntity->getPosition()->getY() > planes->getMax(Y)) || (otherEntity->getPosition()->getY() < planes->getMin(Y))) {
			otherEntity->getVelocity()->setY(0);
		} else {
			otherEntity->getVelocity()->setX(0);
			otherEntity->getVelocity()->setZ(0);
		}
		return true;
	}
	return false;
}

void BlockEntity::drawSelf() {
	planes->drawSelf();
}
