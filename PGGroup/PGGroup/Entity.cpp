#include "Entity.h"
#include "ColliderLinkedList.h"

Entity::Entity(ThreeAxis* aPosition, GLuint *aTexture, GLuint aVBO) {
	position = aPosition;
	rotation = new ThreeAxis(0.0, 0.0, 0.0);
	velocity = new ThreeAxis(0.0, 0.0, 0.0);
	texture = aTexture;
	vbo = aVBO;
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
ColliderLinkedList* Entity::getColliders() { return colliders; }
