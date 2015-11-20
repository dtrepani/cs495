#include "Entity.h"

Entity::Entity(float posX, float posY, float posZ,
				GLuint *aTexture, GLuint aVBO) {
	position = new ThreeAxis(posX, posY, posZ);
	rotation = new ThreeAxis(0.0, 0.0, 0.0);
	velocity = new ThreeAxis(0.0, 0.0, 0.0);
	texture = aTexture;
	vbo = aVBO;
}

Entity::~Entity(void) {
	delete position;
	delete rotation;
	delete velocity;
	delete texture;
	delete colliders;
}

bool Entity::hasCollided(LinkedList* otherColliders) {
	return colliders->hasCollided(otherColliders);
}

ThreeAxis* Entity::getPosition() { return position; }
LinkedList* Entity::getColliders() { return colliders; }
