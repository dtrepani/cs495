#include "PlaneEntity.h"

PlaneEntity::PlaneEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, Orientation aOrientation)
: Entity(aPosition, aTexture, aVertices) {
	orientation = aOrientation;
}

PlaneEntity::~PlaneEntity(void) {}

// Find the smallest value of an axis to determine the lower boundary for the plane.
float PlaneEntity::getSmallestPositionValFor(int axis) {
	float smallest = vertices[axis];
	for(int i = 1; i < 4; i++) {
		if(vertices[i*3 + axis] < smallest) {
			smallest = vertices[i*3 + axis];
		}
	}
	return smallest + ( (axis == 0) ? position->getX() : (axis == 1) ? position->getY() : position->getZ() );
}

// Find the biggest value of an axis to determine the upper boundary for the plane.
float PlaneEntity::getBiggestPositionValFor(int axis) {
	float greatest = vertices[axis];
	for(int i = 1; i < 4; i++) {
		if(vertices[i*3 + axis] > greatest) {
			greatest = vertices[i*3 + axis];
		}
	}
	return greatest + ( (axis == 0) ? position->getX() : (axis == 1) ? position->getY() : position->getZ() );
}

// Check if an entity is within the plane's boundaries. Without this, a plane is considered infinite when checking for collisions.
bool PlaneEntity::entityWithinPlaneBoundaries(Vector* otherPosition) {
	if(orientation == VERTICAL_X) {
		return ( (otherPosition->getX() < getBiggestPositionValFor(X) && otherPosition->getX() > getSmallestPositionValFor(X)) &&
				 (otherPosition->getY() < getBiggestPositionValFor(Y) && otherPosition->getY() > getSmallestPositionValFor(Y)) );
	} else if(orientation == VERTICAL_Z) {
		return ( (otherPosition->getY() < getBiggestPositionValFor(Y) && otherPosition->getY() > getSmallestPositionValFor(Y)) &&
				 (otherPosition->getZ() < getBiggestPositionValFor(Z) && otherPosition->getZ() > getSmallestPositionValFor(Z)) );
	} else {
		return ( (otherPosition->getX() < getBiggestPositionValFor(X) && otherPosition->getX() > getSmallestPositionValFor(X)) &&
				 (otherPosition->getZ() < getBiggestPositionValFor(Z) && otherPosition->getZ() > getSmallestPositionValFor(Z)) );
	}
}

// Each plane orientation has their own axis to check if an entity has collided with it or not.
// Planes must also check if the entity is within its boundaries or else their other checks would be an infinite field.
bool PlaneEntity::hasCollided(Entity* otherEntity) {
	Vector* otherPosition = otherEntity->getPosition();

	if(!entityWithinPlaneBoundaries(otherPosition)) {
		return false;
	}

	if(orientation == VERTICAL_X) {
		return (abs(position->getZ() - otherPosition->getZ()) < SENSITIVITY * 3.0f);
	} else if(orientation == VERTICAL_Z) {
		return (abs(position->getX() - otherPosition->getX()) < SENSITIVITY * 3.0f);
	} else {
		return (abs(position->getY() - otherPosition->getY()) < SENSITIVITY * 5.0f); // TO-DO: Fix this filthy hack that's for the player entity only
	}
}

// Each plane orientation has their own axis to check if an entity is moving toward it or not.
bool PlaneEntity::isMovingToward(Entity* otherEntity) {
	Vector* otherPosition = otherEntity->getPosition();
	Vector* otherPositionWithVelocity = otherPosition->add(otherEntity->getVelocity());

	if(orientation == VERTICAL_X) {
		return ( abs(position->getZ() - otherPosition->getZ()) > abs(position->getZ() - otherPositionWithVelocity->getZ()) );
	} else if(orientation == VERTICAL_Z) {
		return ( abs(position->getX() - otherPosition->getX()) > abs(position->getX() - otherPositionWithVelocity->getX()) );
	} else {
		return ( abs(position->getY() - otherPosition->getY()) > abs(position->getY() - otherPositionWithVelocity->getY()) );
	}
}

// Plane entities do not need to zero out the entire velocity vector. This would prevent any kind of movement
// if collided with the floor. Thus, only certain axis are zero'd out depending on the plane's orientation.
void PlaneEntity::checkForCollision(Entity* otherEntity) {
	if(hasCollided(otherEntity) && isMovingToward(otherEntity)) {
		if(orientation == VERTICAL_X || orientation == VERTICAL_Z) {
			otherEntity->getVelocity()->setX(0);
			otherEntity->getVelocity()->setZ(0);
		}
		else {
			otherEntity->getVelocity()->setY(0);
		}
	}
}
