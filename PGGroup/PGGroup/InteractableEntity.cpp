#include "InteractableEntity.h"

InteractableEntity::InteractableEntity(ThreeAxis* aPosition, GLuint *aTexture, GLfloat* aVertices)
: Entity(aPosition, aTexture, aVertices) {
}

InteractableEntity::~InteractableEntity(void) {}
