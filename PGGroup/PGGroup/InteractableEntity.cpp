#include "InteractableEntity.h"

InteractableEntity::InteractableEntity(ThreeAxis* aPosition, GLuint *aTexture, GLuint aVBO)
: Entity(aPosition, aTexture, aVBO) {
}

InteractableEntity::~InteractableEntity(void) {}
