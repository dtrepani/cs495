#include "ColliderEntity.h"

// If aRadius is 0, will use a default value.
ColliderEntity::ColliderEntity(Vector* aPosition, GLuint* aTexture, GLfloat* aVertices, float aRadius, Vector* anEntityPosition, bool aIsPlayer)
: Entity(aPosition, aTexture, aVertices) {
	radius = ((aRadius == 0.0) ? 0.8f : aRadius);
	entityPosition = anEntityPosition;
	isPlayer = aIsPlayer;
}

ColliderEntity::~ColliderEntity(void) {}

// Positions are the collider entity are relative to the entityPosition.
// Asking for the collider's position requires the two be added together for the absolute position.
// The Player's coordinates are opposite all other entities, so its position must be subtracted to get the absolute position.
Vector* ColliderEntity::getPosition() {
	if(isPlayer) {
		return new Vector(
			entityPosition->getX() - position->getX(),
			entityPosition->getY() - position->getY(),
			entityPosition->getZ() - position->getZ());
	} else {
		return new Vector(
			entityPosition->getX() + position->getX(),
			entityPosition->getY() + position->getY(),
			entityPosition->getZ() + position->getZ());
	}
}

float ColliderEntity::getRadius() { return radius; }
