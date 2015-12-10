#include "PlaneEntity.h"

PlaneEntity::PlaneEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, Orientation aOrientation)
: Entity(aPosition, aTexture, aVertices, 0) {
	orientation = aOrientation;
}

PlaneEntity::~PlaneEntity(void) {}

// Find the smallest value of an axis to determine the lower boundary for the plane.
float PlaneEntity::getMin(int axis) {
	Vector* pos = getPosition();
	float smallest = vertices[axis];
	for(int i = 1; i < 4; i++) {
		if(vertices[i*3 + axis] < smallest) {
			smallest = vertices[i*3 + axis];
		}
	}
	smallest = smallest + ( (axis == 0) ? pos->getX() : (axis == 1) ? pos->getY() : pos->getZ() );
	if(parent) delete pos;
	return smallest;
}

// Find the biggest value of an axis to determine the upper boundary for the plane.
float PlaneEntity::getMax(int axis) {
	Vector* pos = getPosition();
	float greatest = vertices[axis];
	for(int i = 1; i < 4; i++) {
		if(vertices[i*3 + axis] > greatest) {
			greatest = vertices[i*3 + axis];
		}
	}
	greatest = greatest + ( (axis == 0) ? pos->getX() : (axis == 1) ? pos->getY() : pos->getZ() );
	if(parent) delete pos;
	return greatest;
}

// Check if an entity is within the plane's boundaries. Without this, a plane is considered infinite when checking for collisions.
bool PlaneEntity::withinPlaneBoundaries(Entity* otherEntity) {
	Vector* otherPosition = otherEntity->getPosition();
	float otherRadius = otherEntity->getRadius();

	if(orientation == VERTICAL_X) {
		return ( (otherPosition->getX() < getMax(X) && otherPosition->getX() > getMin(X)) &&
				 (otherPosition->getY() < getMax(Y) && otherPosition->getY() > getMin(Y)) );
	} else if (orientation == VERTICAL_Z) {
		return ( (otherPosition->getY() < getMax(Y) && otherPosition->getY() > getMin(Y)) &&
				 (otherPosition->getZ() < getMax(Z) && otherPosition->getZ() > getMin(Z)) );
	} else if (orientation == HORIZONTAL) {
		return ( (otherPosition->getX() < getMax(X) && otherPosition->getX() > getMin(X)) &&
				 (otherPosition->getZ() < getMax(Z) && otherPosition->getZ() > getMin(Z)) );
	} else {
		return false;
	}
}

// Each plane orientation has their own axis to check if an entity has collided with it or not.
// Planes must also check if the entity is within its boundaries or else their other checks would be an infinite field.
bool PlaneEntity::hasCollided(Entity* otherEntity) {
	Vector* pos = getPosition();
	Vector* otherPosition = otherEntity->getPosition();
	bool collision = false;

	if((withinPlaneBoundaries(otherEntity))) {
		if(orientation == VERTICAL_X) {
			collision = (abs(position->getZ() - otherPosition->getZ()) <= otherEntity->getRadius());
		} else if(orientation == VERTICAL_Z) {
			collision = (abs(position->getX() - otherPosition->getX()) <= otherEntity->getRadius());
		} else if(orientation == HORIZONTAL) {
			collision = (abs(position->getY() - otherPosition->getY()) <= otherEntity->getRadius());
		}
	}

	if(parent) delete pos;
	return collision;
}

// Each plane orientation has their own axis to check if an entity is moving toward it or not.
bool PlaneEntity::isMovingToward(Entity* otherEntity) {
	Vector* pos = getPosition();
	Vector* otherPosition = otherEntity->getPosition();
	Vector* otherPositionWithVelocity = otherPosition->add(otherEntity->getVelocity());
	bool movingToward = false;

	if(orientation == VERTICAL_X) {
		movingToward = ( abs(pos->getZ() - otherPosition->getZ()) > abs(pos->getZ() - otherPositionWithVelocity->getZ()) );
	} else if(orientation == VERTICAL_Z) {
		movingToward = ( abs(pos->getX() - otherPosition->getX()) > abs(pos->getX() - otherPositionWithVelocity->getX()) );
	} else if(orientation == HORIZONTAL) {
		movingToward = ( abs(pos->getY() - otherPosition->getY()) > abs(pos->getY() - otherPositionWithVelocity->getY()) );
	}

	if(parent) delete pos;
	return movingToward;
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
/*
void PlaneEntity::drawSelf() {
	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0, opacity);
	glBindTexture( GL_TEXTURE_2D, *texture );
	
	addVelocityToPosition();

	glTranslatef(position->getX(), position->getY(), position->getZ());
	rotateEntity();
	glScalef(scale->getX(), scale->getY(), scale->getZ());

	float texX = 1.0f,
		  texY = 1.0f;

	// TO-DO: Remove. Testing tiles.
	if(orientation == VERTICAL_X) { // x y
		texY = 128.0f / getMax(X);
		texX = 128.0f / getMax(Y);
	} else if(orientation == VERTICAL_Z) { // y z
		texY = 128.0f / getMax(Y);
		texX = 128.0f / getMax(Z);
	} else if(orientation == HORIZONTAL) { // x z
		texY = 128.0f / getMax(X);
		texX = 128.0f / getMax(Z);
	}

	glBegin(GL_QUADS);
		glTexCoord2f(0, texY);		glVertex3f(vertices[0], vertices[1], vertices[2]);
		glTexCoord2f(texX, texY);	glVertex3f(vertices[3], vertices[4], vertices[5]);
		glTexCoord2f(texX, 0);		glVertex3f(vertices[6], vertices[7], vertices[8]);
		glTexCoord2f(0, 0);			glVertex3f(vertices[9], vertices[10], vertices[11]);
	glEnd();
	glPopMatrix();
}*/
