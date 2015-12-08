#ifndef _COININTERACTABLEENTITY_
#define _COININTERACTABLEENTITY_

#include "InteractableEntity.h"

class CoinInteractableEntity : public InteractableEntity {
private:

public:
	CoinInteractableEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius);
	~CoinInteractableEntity(void);
	void interactedWith();
};

#endif
