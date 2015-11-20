#include "PlayerEntity.h"

PlayerEntity::PlayerEntity(ThreeAxis* aPosition, GLuint *aTexture, GLuint aVBO)
: Entity(aPosition, aTexture, aVBO) {
	state = STANDING;
	interact = false;
	health = 100; // can change later if wanted ?
}

PlayerEntity::~PlayerEntity(void) {
}

void PlayerEntity::pain(int hurt){
	health -= hurt;
	if (health <= 0)
		state = DEAD;
}

void PlayerEntity::toggleInteract(){
	interact = !interact;
}

void PlayerEntity::jump(){
	state = JUMPING;
	//velocity.y = NEED TO FIND VARIABLE THAT WORKS SMOOTHLY
}
