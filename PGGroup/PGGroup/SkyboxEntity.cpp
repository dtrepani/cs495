#include "SkyboxEntity.h"
#include "Level.h"

SkyboxBlockEntity::SkyboxBlockEntity()
: BlockEntity(new Vector(0, 0, 0), NULL, 0, 0, 0) {
	textureNum = 0;
	float widthX, heightY, lengthZ;
	widthX = heightY = lengthZ = 100.0f;

	GLfloat frontFace[12] = {
		 widthX, -heightY,  lengthZ,
		-widthX, -heightY,  lengthZ,
		-widthX,  heightY,  lengthZ,
		 widthX,  heightY,  lengthZ,
	};
	createAndAdd(&frontFace[0], VERTICAL_X);

	GLfloat backFace[12] = {
		 widthX, -heightY, -lengthZ,
		-widthX, -heightY, -lengthZ,
		-widthX,  heightY, -lengthZ,
		 widthX,  heightY, -lengthZ,
	};
	createAndAdd(&backFace[0], VERTICAL_X);

	GLfloat topFace[12] = {
		-widthX,  heightY,  lengthZ,
		 widthX,  heightY,  lengthZ,
		 widthX,  heightY, -lengthZ,
		-widthX,  heightY, -lengthZ,
	};
	createAndAdd(&topFace[0], HORIZONTAL);

	GLfloat bottomFace[12] = {
		-widthX, -heightY,  lengthZ,
		 widthX, -heightY,  lengthZ,
		 widthX, -heightY, -lengthZ,
		-widthX, -heightY, -lengthZ,
	};
	createAndAdd(&bottomFace[0], HORIZONTAL);

	GLfloat rightFace[12] = {
		 widthX, -heightY, -lengthZ,
		 widthX, -heightY,  lengthZ,
		 widthX,  heightY,  lengthZ,
		 widthX,  heightY, -lengthZ,
	};
	createAndAdd(&rightFace[0], VERTICAL_Z);

	GLfloat leftFace[12] = {
		-widthX, -heightY, -lengthZ,
		-widthX, -heightY,  lengthZ,
		-widthX,  heightY,  lengthZ,
		-widthX,  heightY, -lengthZ,
	};
	createAndAdd(&leftFace[0], VERTICAL_Z);
}

SkyboxBlockEntity::~SkyboxBlockEntity(void) { }

void SkyboxBlockEntity::createAndAdd(GLfloat* vertices, Orientation orientation) {
	planes->add(new PlaneEntity(new Vector(0, 0, 0), Level::createTexture("skybox" + to_string(textureNum)), &vertices[0], orientation));
	textureNum++;
}
