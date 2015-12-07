#ifndef _WIZARDENTITY_
#define _WIZARDENTITY_

#include "Entity.h"

class WizardEntity : public Entity {
private:
	typedef enum {
		FLOATING,
		CASTING,
		DEAD
	} Status;

	int health;
	Status state;

public:
	WizardEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius);
	~WizardEntity(void);
	void pain(int hurt);
};

#endif
