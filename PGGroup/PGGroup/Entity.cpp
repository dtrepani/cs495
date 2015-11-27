#include "Entity.h"
#include "ColliderLinkedList.h"

Entity::Entity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices) {
	position = aPosition;
	rotation = new Vector(0.0, 0.0, 0.0);
	velocity = new Vector(0.0, 0.0, 0.0);
	texture = aTexture;
	opacity = 0;
	vertices = aVertices;
	colliders = new ColliderLinkedList();
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
bool Entity::hasCollided(Entity* otherEntity) { return colliders->hasCollided(otherEntity->getColliders()); }

// Rotate the entity according to its rotation variables.
void Entity::rotateEntity() {
	glRotatef(rotation->getX(), 1, 0, 0);
	glRotatef(rotation->getY(), 0, 1, 0);
	glRotatef(rotation->getZ(), 0, 0, 1);
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

// Set the x, y, and z values of any location information
void Entity::setXYZOf(LocationInfo locInfo, float x, float y, float z) {
	getCorrespondingVector(locInfo)->setX(x);
	getCorrespondingVector(locInfo)->setY(y);
	getCorrespondingVector(locInfo)->setZ(z);
}

Vector* Entity::getPosition() { return position; }
Vector* Entity::getRotation() { return rotation; }
Vector* Entity::getVelocity() { return velocity; }
ColliderLinkedList* Entity::getColliders() { return colliders; }

// The entity knows how to draw itself and where to draw itself.
void Entity::drawSelf() {
	glPushMatrix();
	glBindTexture( GL_TEXTURE_2D, *texture );
	
	glTranslatef(position->getX(), position->getY(), position->getZ());
	rotateEntity();

	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);	glVertex3f(vertices[0], vertices[1], vertices[2]);
		glTexCoord2f(1, 1);	glVertex3f(vertices[3], vertices[4], vertices[5]);
		glTexCoord2f(1, 0);	glVertex3f(vertices[6], vertices[7], vertices[8]);
		glTexCoord2f(0, 0);	glVertex3f(vertices[9], vertices[10], vertices[11]);
	glEnd();
	glPopMatrix();
}