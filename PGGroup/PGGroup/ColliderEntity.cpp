#include "ColliderEntity.h"

// If aRadius is 0, will use a default value.
ColliderEntity::ColliderEntity(ThreeAxis* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius, ThreeAxis *anEntityPosition)
: Entity(aPosition, aTexture, aVertices) {
	radius = ((aRadius == 0.0) ? 3.0f : aRadius);
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

float ColliderEntity::getRadius() { return radius; }
