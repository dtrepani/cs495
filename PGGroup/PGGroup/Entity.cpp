#include "Entity.h"
#include "ColliderLinkedList.h"
#include "PlaneEntity.h"

int cntTmpI = 0; // TO-DO: remove

Entity::Entity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices) {
	position = aPosition;
	rotation = new Vector(0.0, 0.0, 0.0);
	velocity = new Vector(0.0, 0.0, 0.0);
	texture = aTexture;
	opacity = 0;
	colliderRadius = 0.8f; // TO-DO: Add radius param
	colliders = new ColliderLinkedList();

	if(aVertices) {
		memcpy(&vertices[0], &aVertices[0], sizeof(vertices));
	} else {
		GLfloat vertZero[12] = { 0 };
		memcpy(&vertices[0], &vertZero[0], sizeof(vertices));
	}

	tmpi = cntTmpI;
	cntTmpI++;
}

Entity::~Entity(void) {
	delete position;
	delete rotation;
	delete velocity;
	delete colliders;
}

// Add a collider entity to the list of colliders
void Entity::addCollider(float x, float y, float z, float radius) { colliders->add(new ColliderEntity(new Vector(x, y, z), NULL, NULL, radius, position, false)); }

// Check if this entity has collided with another entity by comparing their colliders.
bool Entity::hasCollided(Entity* otherEntity) {	
	return (position->distanceTo(otherEntity->getPosition()) <= (colliderRadius + otherEntity->getRadius()));
}

// Check if this entity has collided with a plane entity.
// The plane entity doesn't use colliders, but has its own implementation of hasCollided so let it handle checking
// if it's collided with this entity.
bool Entity::hasCollided(PlaneEntity* otherEntity) { return otherEntity->hasCollided(this); }

// Check if entity has collided with another entity and stop further velocity if it has.
// If the entity is not moving toward it (i.e., trying to get away), allow it to move.
bool Entity::checkForCollision(Entity* otherEntity) {
	bool collisionAndMovingToward = hasCollided(otherEntity) && isMovingToward(otherEntity);
	if(collisionAndMovingToward) {
		velocity->zero();
	}
	return collisionAndMovingToward;
}

// Check if entity has collided with a plane entity and stop further velocity if it has.
// The plane entity doesn't use colliders, but has its own implementation of checkForCollisions so let it handle checking
// if it's collided with this entity.
bool Entity::checkForCollision(PlaneEntity* otherEntity) {
	return otherEntity->checkForCollision(this);
}

// Rotate the entity according to its rotation variables.
void Entity::rotateEntity() {
	glRotatef(rotation->getX(), 1, 0, 0);
	glRotatef(rotation->getY(), 0, 1, 0);
	glRotatef(rotation->getZ(), 0, 0, 1);
}

// Check if this entity is moving toward another by comparing the distance to the other entity at its current position to the
// distance to the other entity at its new position. If the latter is less than the first, its moving toward the other entity.
bool Entity::isMovingToward(Entity* otherEntity) { return (position->distanceTo(otherEntity->getPosition()) > ((position->add(velocity))->distanceTo(otherEntity->getPosition())) ); }
bool Entity::isMovingToward(PlaneEntity* otherEntity) { return otherEntity->isMovingToward(this); }

// Check if this entity is within a plane's boundaries by comparing the colliders to the plane.
//bool Entity::withinPlaneBoundaries(PlaneEntity* plane) { return colliders->withinPlaneBoundaries(plane); }
bool Entity::withinPlaneBoundaries(PlaneEntity* plane) {
	Orientation planeOrientation = plane->getOrientation();

	if(planeOrientation == VERTICAL_X) {
		return ( (position->getX()-colliderRadius < plane->getMax(X) && position->getX()+colliderRadius > plane->getMin(X)) &&
				 (position->getY()-colliderRadius < plane->getMax(Y) && position->getY()+colliderRadius > plane->getMin(Y)) );
	} else if (planeOrientation == VERTICAL_Z) {
		return ( (position->getY()-colliderRadius < plane->getMax(Y) && position->getY()+colliderRadius > plane->getMin(Y)) &&
				 (position->getZ()-colliderRadius < plane->getMax(Z) && position->getZ()+colliderRadius > plane->getMin(Z)) );
	} else if (planeOrientation == HORIZONTAL) {
		return ( (position->getX() < plane->getMax(X) && position->getX() > plane->getMin(X)) &&
				 (position->getZ() < plane->getMax(Z) && position->getZ() > plane->getMin(Z)) );
	} else {
		return false;
	}
}


// Return the Vector location information based on its corresponding enum.
Vector* Entity::getCorrespondingVector(LocationInfo locationInfo) {
	return (locationInfo == POSITION) ? position 
									  : (locationInfo == ROTATION) ? rotation : velocity;
}

// Increment the values for use in creating fluid movement.
void Entity::incrementXOf(LocationInfo locInfo, float x) { getCorrespondingVector(locInfo)->incrementX(x); }
void Entity::incrementYOf(LocationInfo locInfo, float y) { getCorrespondingVector(locInfo)->incrementY(y); }
void Entity::incrementZOf(LocationInfo locInfo, float z) { getCorrespondingVector(locInfo)->incrementZ(z); }

Vector* Entity::getPosition() { return position; }
Vector* Entity::getRotation() { return rotation; }
Vector* Entity::getVelocity() { return velocity; }
ColliderLinkedList* Entity::getColliders() { return colliders; }

float Entity::getRadius() { return colliderRadius; }

// The entity knows how to draw itself and where to draw itself.
void Entity::drawSelf() {
	glPushMatrix();
	glBindTexture( GL_TEXTURE_2D, *texture );
	
	glTranslatef(position->getX(), position->getY(), position->getZ());
	rotateEntity();

	glBegin(GL_QUADS);
		glTexCoord2f(0, 1.0f);		glVertex3f(vertices[0], vertices[1], vertices[2]);
		glTexCoord2f(1.0f, 1.0f);	glVertex3f(vertices[3], vertices[4], vertices[5]);
		glTexCoord2f(1.0f, 0);		glVertex3f(vertices[6], vertices[7], vertices[8]);
		glTexCoord2f(0, 0);			glVertex3f(vertices[9], vertices[10], vertices[11]);
	glEnd();
	glPopMatrix();
}