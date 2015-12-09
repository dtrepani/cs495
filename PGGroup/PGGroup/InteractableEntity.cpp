#include "InteractableEntity.h"
#include "PlayerEntity.h"

InteractableEntity::InteractableEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius)
: Entity(aPosition, aTexture, aVertices, aRadius) {
	interactRadius = (aRadius ? aRadius*2.0f : 1.6f);
}

InteractableEntity::~InteractableEntity(void) {}

void InteractableEntity::interactWith(PlayerEntity* player) {}

// Check if this entity has collided with another entity by comparing their colliders.
bool InteractableEntity::hasCollidedInteract(Entity* otherEntity) {	
	return (position->distanceTo(otherEntity->getPosition()) <= (interactRadius + otherEntity->getRadius()));
}

// Requires the otherEntity be a PlayerEntity in order to toggle interaction.
bool InteractableEntity::checkForCollision(Entity* otherEntity) {
	bool collisionAndMovingToward = hasCollided(otherEntity) && isMovingToward(otherEntity);

	if(collisionAndMovingToward && !passable)
		otherEntity->getVelocity()->zero();
	
	if(hasCollidedInteract(otherEntity))
		((PlayerEntity*)otherEntity)->setInteract(this, this);
	else
		((PlayerEntity*)otherEntity)->setInteract(this, NULL);

	return collisionAndMovingToward;
}
