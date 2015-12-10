#include "GumballMachineInteractableEntity.h"
#include "PlayerEntity.h"

GumballMachineInteractableEntity::GumballMachineInteractableEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius)
: InteractableEntity(aPosition, aTexture, aVertices, aRadius) {
	passable = true;
}

GumballMachineInteractableEntity::~GumballMachineInteractableEntity(void) {}

void GumballMachineInteractableEntity::interactWith(PlayerEntity* player) {
	if(player->hasPickedUpCoin()) {
		player->showGumball(true);
	}
}
