#include "Entity.h"

//Entity::Entity() {
//};

Entity::Entity(float posX, float posY, float posZ,
				GLuint *aTexture, GLuint aVBO) {
	position.x = posX;
	position.y = posY;
	position.z = posZ;
	velocity.x = 0;
	velocity.y = 0;
	velocity.z = 0;
	texture = aTexture;
	vbo = aVBO;
}


Entity::~Entity(void) {
}
