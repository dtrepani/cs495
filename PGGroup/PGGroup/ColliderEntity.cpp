#include "ColliderEntity.h"

ColliderEntity::ColliderEntity(ThreeAxis* aPosition, GLuint *aTexture, GLuint aVBO, int aRadius, ThreeAxis *anEntityPosition)
: Entity(aPosition, aTexture, aVBO) {
	radius = aRadius;
	entityPosition = anEntityPosition;
}

ColliderEntity::~ColliderEntity(void) {}

// Positions are the collider entity are relative to the entityPosition.
// Asking for the collider's position requires the two be added together for the absolute position.
ThreeAxis* ColliderEntity::getPosition() {
	return new ThreeAxis(
		entityPosition->getX() + position->getX(),
		entityPosition->getY() + position->getY(),
		entityPosition->getZ() + position->getZ());
}

int ColliderEntity::getRadius() { return radius; }
