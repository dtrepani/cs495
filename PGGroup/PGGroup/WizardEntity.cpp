#include "WizardEntity.h"

WizardEntity::WizardEntity(ThreeAxis* aPosition, GLuint *aTexture, GLuint aVBO)
: Entity(aPosition, aTexture, aVBO) {
	state = FLOATING;
	health = 100;
}

WizardEntity::~WizardEntity(void) {}

void WizardEntity::pain(int hurt){
	health -= hurt;
	if (health <= 0)
		state = DEAD;
}


