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

	int health, frame;
	float floatingAngle;
	GLint initialAnimTime;
	Status state;

	void animate();
	void animFloat();
	void animDeath();
	void animCast();
	bool timePassedGreaterThan(int ms);

public:
	WizardEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius);
	~WizardEntity(void);
	void pain(int hurt);
	bool checkForCollision(Entity* otherEntity);
	void drawSelf();
};

#endif
