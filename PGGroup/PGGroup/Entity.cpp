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

bool Entity::hasCollided(ColliderLinkedList* otherColliders) {
	return colliders->hasCollided(otherColliders);
}

ThreeAxis* Entity::getPosition() { return position; }
ColliderLinkedList* Entity::getColliders() { return colliders; }
