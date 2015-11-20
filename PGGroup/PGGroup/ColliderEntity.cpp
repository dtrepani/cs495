#include "ColliderEntity.h"

ColliderEntity::ColliderEntity(int aRadius, float posX, float posY, float posZ, GLuint *aTexture, GLuint aVBO)
: Entity(posX, posY, posZ, aTexture, aVBO) {
	radius = aRadius;
}

ColliderEntity::~ColliderEntity(void) {
}

int ColliderEntity::getRadius() { return radius; }
