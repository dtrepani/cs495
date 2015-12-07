#include "PlaneEntity.h"
#include <cstdlib>
#include <time.h>


PlaneEntity::PlaneEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, Orientation aOrientation)
: Entity(aPosition, aTexture, aVertices) {
	srand (time(NULL));
	orientation = aOrientation;
	pointOnPlane = new Vector( getRandValOnPlane(X), getRandValOnPlane(Y), getRandValOnPlane(Z) );
}

PlaneEntity::~PlaneEntity(void) {}

// Get a random point on the plane for an axis.
float PlaneEntity::getRandValOnPlane(int axis) {
	float min = getMin(axis);
	float max = getMax(axis);
    float randVal = (float)rand() / RAND_MAX;
    return min + randVal * (max - min);
}

// Find the smallest value of an axis to determine the lower boundary for the plane.
float PlaneEntity::getMin(int axis) {
	float smallest = vertices[axis];
	for(int i = 1; i < 4; i++) {
		if(vertices[i*3 + axis] < smallest) {
			smallest = vertices[i*3 + axis];
		}
	}
	return smallest + ( (axis == 0) ? position->getX() : (axis == 1) ? position->getY() : position->getZ() );
}

// Find the biggest value of an axis to determine the upper boundary for the plane.
float PlaneEntity::getMax(int axis) {
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
		return ( (otherPosition->getX() < getMax(X) && otherPosition->getX() > getMin(X)) && // TO-DO: should consider other entity's collider's radius
				 (otherPosition->getY() < getMax(Y) && otherPosition->getY() > getMin(Y)) );
	} else if(orientation == VERTICAL_Z) {
		return ( (otherPosition->getY() < getMax(Y) && otherPosition->getY() > getMin(Y)) &&
				 (otherPosition->getZ() < getMax(Z) && otherPosition->getZ() > getMin(Z)) );
	} else if(orientation == HORIZONTAL) {
		return ( (otherPosition->getX() < getMax(X) && otherPosition->getX() > getMin(X)) &&
				 (otherPosition->getZ() < getMax(Z) && otherPosition->getZ() > getMin(Z)) );
	} else {
		return false;
	}
}

// Each plane orientation has their own axis to check if an entity has collided with it or not.
// Planes must also check if the entity is within its boundaries or else their other checks would be an infinite field.
bool PlaneEntity::hasCollided(Entity* otherEntity) {
	Vector* otherPosition = otherEntity->getPosition();

	if(!(otherEntity->withinPlaneBoundaries(this))) {
	//if(!(entityWithinPlaneBoundaries(otherPosition))) {
		return false;
	}

	if(orientation == VERTICAL_X) {
		return (abs(position->getZ() - otherPosition->getZ()) < otherEntity->getRadius());
	} else if(orientation == VERTICAL_Z) {
		return (abs(position->getX() - otherPosition->getX()) < otherEntity->getRadius());
	} else if(orientation == HORIZONTAL) {
		return (abs(position->getY() - otherPosition->getY()) < otherEntity->getRadius());
	} else {
		return false;
	}

	/*
	Vector* otherPosition = otherEntity->getPosition();
	Vector* planeNormal = position->normalize();
	float distFromOrigin = planeNormal->distanceTo(pointOnPlane);
	float distance = planeNormal->dotProduct( otherPosition ) + distFromOrigin;
	if( abs(distance) <= 1.0f ) return true; // TO-DO: tmp value
	else return false;*/
}

// Each plane orientation has their own axis to check if an entity is moving toward it or not.
bool PlaneEntity::isMovingToward(Entity* otherEntity) {
	Vector* otherPosition = otherEntity->getPosition();
	Vector* otherPositionWithVelocity = otherPosition->add(otherEntity->getVelocity());

	if(orientation == VERTICAL_X) {
		return ( abs(position->getZ() - otherPosition->getZ()) > abs(position->getZ() - otherPositionWithVelocity->getZ()) );
	} else if(orientation == VERTICAL_Z) {
		return ( abs(position->getX() - otherPosition->getX()) > abs(position->getX() - otherPositionWithVelocity->getX()) );
	} else if(orientation == HORIZONTAL) {
		return ( abs(position->getY() - otherPosition->getY()) > abs(position->getY() - otherPositionWithVelocity->getY()) );
	} else {
		return false;
	}
}

// Plane entities do not need to zero out the entire velocity vector. This would prevent any kind of movement
// if collided with the floor. Thus, only certain axis are zero'd out depending on the plane's orientation.
bool PlaneEntity::checkForCollision(Entity* otherEntity) {
	bool collisionAndMovingToward = hasCollided(otherEntity) && isMovingToward(otherEntity);
	if(collisionAndMovingToward) {
		if(orientation == VERTICAL_X || orientation == VERTICAL_Z) {
			otherEntity->getVelocity()->setX(0);
			otherEntity->getVelocity()->setZ(0);
		}
		else if (orientation == HORIZONTAL) {
			otherEntity->getVelocity()->setY(0);
		}
	}
	return collisionAndMovingToward;
}

Orientation PlaneEntity::getOrientation() { return orientation; }
