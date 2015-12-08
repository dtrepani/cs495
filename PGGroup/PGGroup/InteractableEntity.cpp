#include "InteractableEntity.h"

InteractableEntity::InteractableEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius)
: Entity(aPosition, aTexture, aVertices, aRadius) {
}

InteractableEntity::~InteractableEntity(void) {}

