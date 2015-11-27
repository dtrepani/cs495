#include "InteractableEntity.h"

InteractableEntity::InteractableEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices)
: Entity(aPosition, aTexture, aVertices) {
}

InteractableEntity::~InteractableEntity(void) {}
