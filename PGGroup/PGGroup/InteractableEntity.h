#ifndef _INTERACTABLEENTITY_
#define _INTERACTABLEENTITY_

#include "Entity.h"

class InteractableEntity : public Entity {
private:

public:
	InteractableEntity(ThreeAxis* aPosition, GLuint *aTexture, GLfloat* aVertices);
	~InteractableEntity(void);
	//virtual void interactedWith();
};

#endif
