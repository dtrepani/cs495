#include "Entity.h"
#include "PlaneEntity.h"

Entity::Entity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius) {
	position = aPosition;
	rotation = new Vector(0.0, 0.0, 0.0);
	velocity = new Vector(0.0, 0.0, 0.0);
	scale = new Vector(1.0f, 1.0f, 1.0f);
	texture = aTexture;
	radius = ((aRadius == NULL) ? 0.8f : aRadius);
	opacity = 1.0f;

	if(aVertices) {
		memcpy(&vertices[0], &aVertices[0], sizeof(vertices));
	} else {
		GLfloat vertZero[12] = { 0 };
		memcpy(&vertices[0], &vertZero[0], sizeof(vertices));
	}
}

Entity::~Entity(void) {
	delete position;
	delete rotation;
	delete velocity;
	delete scale;
}

// Check if this entity has collided with another entity by comparing their colliders.
bool Entity::hasCollided(Entity* otherEntity) {	
	return (position->distanceTo(otherEntity->getPosition()) <= (radius + otherEntity->getRadius()));
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

// Adds the velocity to the entity's position and reset velocity.
void Entity::addVelocityToPosition() {
	Vector* tmp = position;
	position = position->add(velocity);
	velocity->zero();
	delete tmp;
}

// Return the Vector location information based on its corresponding enum.
Vector* Entity::getCorrespondingVector(LocationInfo locationInfo) {
	return (locationInfo == POSITION) ? position : (locationInfo == ROTATION) ? rotation : (locationInfo == VELOCITY) ? velocity : scale;
}

// Increment the values for use in creating fluid movement.
void Entity::incrementXOf(LocationInfo locInfo, float x) { getCorrespondingVector(locInfo)->incrementX(x); }
void Entity::incrementYOf(LocationInfo locInfo, float y) { getCorrespondingVector(locInfo)->incrementY(y); }
void Entity::incrementZOf(LocationInfo locInfo, float z) { getCorrespondingVector(locInfo)->incrementZ(z); }

Vector* Entity::getPosition() { return position; }
Vector* Entity::getRotation() { return rotation; }
Vector* Entity::getVelocity() { return velocity; }
Vector* Entity::getScale() { return scale; }
float Entity::getRadius() { return radius; }

// The entity knows how to draw itself and where to draw itself.
void Entity::drawSelf() {
	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0, opacity);
	glBindTexture( GL_TEXTURE_2D, *texture );
	
	addVelocityToPosition();

	glTranslatef(position->getX(), position->getY(), position->getZ());
	rotateEntity();
	glScalef(scale->getX(), scale->getY(), scale->getZ());

	glBegin(GL_QUADS);
		glTexCoord2f(0, 1.0f);		glVertex3f(vertices[0], vertices[1], vertices[2]);
		glTexCoord2f(1.0f, 1.0f);	glVertex3f(vertices[3], vertices[4], vertices[5]);
		glTexCoord2f(1.0f, 0);		glVertex3f(vertices[6], vertices[7], vertices[8]);
		glTexCoord2f(0, 0);			glVertex3f(vertices[9], vertices[10], vertices[11]);
	glEnd();
	glPopMatrix();
}