#include "Entity.h"
#include "ColliderLinkedList.h"

Entity::Entity(ThreeAxis* aPosition, GLuint *aTexture, GLfloat* aVertices) {
	position = aPosition;
	rotation = new ThreeAxis(0.0, 0.0, 0.0);
	velocity = new ThreeAxis(0.0, 0.0, 0.0);
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
void Entity::addCollider(ColliderEntity* colliderEntity) { colliders->add(colliderEntity); }

// Check if this entity has collided with another entity by comparing their colliders.
bool Entity::hasCollided(ColliderLinkedList* otherColliders) { return colliders->hasCollided(otherColliders); }

// Rotate the entity according to its rotation variables.
void Entity::rotateEntity() {
	glRotatef(rotation->getX(), 1, 0, 0);
	glRotatef(rotation->getY(), 0, 1, 0);
	glRotatef(rotation->getZ(), 0, 0, 1);
}

// Return the ThreeAxis location information based on its corresponding enum.
ThreeAxis* Entity::getCorrespondingThreeAxis(LocationInfo locationInfo) {
	return (locationInfo == POSITION) ? position 
									  : (locationInfo == ROTATION) ? rotation : velocity;
}

// Increment the values for use in creating fluid movement.
void Entity::incrementXOf(LocationInfo aLocInfo, float x) { getCorrespondingThreeAxis(aLocInfo)->incrementX(x); }
void Entity::incrementYOf(LocationInfo aLocInfo, float y) { getCorrespondingThreeAxis(aLocInfo)->incrementY(y); }
void Entity::incrementZOf(LocationInfo aLocInfo, float z) { getCorrespondingThreeAxis(aLocInfo)->incrementZ(z); }

ThreeAxis* Entity::getPosition() { return position; }
ThreeAxis* Entity::getRotation() { return rotation; }
ThreeAxis* Entity::getVelocity() { return velocity; }
ColliderLinkedList* Entity::getColliders() { return colliders; }

// The entity knows how to draw itself and where to draw itself.
void Entity::drawSelf() {
	glBindTexture( GL_TEXTURE_2D, *texture );
	
	glTranslatef(position->getX(), position->getY(), position->getZ());
	rotateEntity();

	glBegin(GL_QUADS);
		glTexCoord2f(0,0);	glVertex3f(vertices[0], vertices[1], vertices[2]);
		glTexCoord2f(1,0);	glVertex3f(vertices[3], vertices[4], vertices[5]);
		glTexCoord2f(1,1);	glVertex3f(vertices[6], vertices[7], vertices[8]);
		glTexCoord2f(0,1);	glVertex3f(vertices[9], vertices[10], vertices[11]);
	glEnd();
}