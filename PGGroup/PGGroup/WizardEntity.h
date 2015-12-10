#ifndef _WIZARDENTITY_
#define _WIZARDENTITY_

#include "Entity.h"

class PlayerEntity;
class WizardEntity : public Entity {
private:
	typedef enum {
		FLOATING,
		CASTING,
		DEAD
	} Status;

	PlayerEntity* player;
	int health, frame, animLocI;
	float floatingAngle;
	GLint initialAnimTime, initialAnimLocTime;
	Status state;
	Vector* animLocs[6]; // Used to store the locations that the wizard will move toward throughout the battle.
	
	void setCast();
	void animate();
	void animMovement();
	void animFloat();
	void animDeath();
	void animCast();
	bool timePassedGreaterThan(int ms);

public:
	WizardEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius, PlayerEntity* aPlayer);
	~WizardEntity(void);
	void pain(int hurt);
	bool checkForCollision(Entity* otherEntity);
	void drawSelf();
};

#endif
