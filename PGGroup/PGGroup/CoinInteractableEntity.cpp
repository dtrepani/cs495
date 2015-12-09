#include "CoinInteractableEntity.h"

CoinInteractableEntity::CoinInteractableEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius)
: InteractableEntity(aPosition, aTexture, aVertices, aRadius) {
	passable = true;
}

CoinInteractableEntity::~CoinInteractableEntity(void) {}

void CoinInteractableEntity::interactWith(PlayerEntity* player) {
	cout << "Interacting with coin!" << endl;
}
