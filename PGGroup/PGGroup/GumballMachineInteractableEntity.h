#ifndef _GUMBALLMACHINEINTERACTABLEENTITY_
#define _GUMBALLMACHINEINTERACTABLEENTITY_

#include "InteractableEntity.h"

class PlayerEntity;
class GumballMachineInteractableEntity : public InteractableEntity {
private:

public:
	GumballMachineInteractableEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius);
	~GumballMachineInteractableEntity(void);
	void interactWith(PlayerEntity* player);
};

#endif
