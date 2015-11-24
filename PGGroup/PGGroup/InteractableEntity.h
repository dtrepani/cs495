#ifndef _INTERACTABLEENTITY_
#define _INTERACTABLEENTITY_

#include "Entity.h"

class InteractableEntity : public Entity {
private:

public:
	InteractableEntity(ThreeAxis* aPosition, GLuint *aTexture, GLuint aVBO);
	~InteractableEntity(void);
	virtual void interactedWith() = 0;
};

#endif
