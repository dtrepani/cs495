#include "CoinInteractableEntity.h"

CoinInteractableEntity::CoinInteractableEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius)
: InteractableEntity(aPosition, aTexture, aVertices, aRadius) {
}

CoinInteractableEntity::~CoinInteractableEntity(void) {}
