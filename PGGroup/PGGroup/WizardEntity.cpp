#include "WizardEntity.h"

WizardEntity::WizardEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius)
: Entity(aPosition, aTexture, aVertices, aRadius) {
	state = FLOATING;
	health = 100;
}

WizardEntity::~WizardEntity(void) {}

void WizardEntity::pain(int hurt){
	health -= hurt;
	if (health <= 0)
		state = DEAD;
}


