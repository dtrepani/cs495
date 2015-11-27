#ifndef _INTERACTABLEENTITY_
#define _INTERACTABLEENTITY_

#include "Entity.h"

class InteractableEntity : public Entity {
private:

public:
	InteractableEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices);
	~InteractableEntity(void);
	//virtual void interactedWith();
};

#endif
