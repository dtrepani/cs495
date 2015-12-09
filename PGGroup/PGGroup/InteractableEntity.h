#ifndef _INTERACTABLEENTITY_
#define _INTERACTABLEENTITY_

#include "Entity.h"

class PlayerEntity;
class InteractableEntity : public Entity {
protected:
	float interactRadius;
	bool hasCollidedInteract(Entity* otherEntity);

public:
	InteractableEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius);
	~InteractableEntity(void);
	virtual void interactWith(PlayerEntity* player);
	bool checkForCollision(Entity* otherEntity);
};

#endif
