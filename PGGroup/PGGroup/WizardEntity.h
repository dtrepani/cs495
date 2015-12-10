#ifndef _WIZARDENTITY_
#define _WIZARDENTITY_

#include "Entity.h"

class LinkedList;
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
	bool shipDestroyed;
	GLint initialAnimTime, initialAnimLocTime;
	Status state;
	Vector* animLocs[6]; // Used to store the locations that the wizard will move toward throughout the battle.
	LinkedList* covers; // Covers to the jumping puzzle
	LinkedList* platforms; // The jp itself
	
	void setCast();
	void animate();
	void animMovement();
	void animFloat();
	void animDeath();
	void animCast();
	bool timePassedGreaterThan(int ms);

public:
	WizardEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius, PlayerEntity* aPlayer, LinkedList* aCovers, LinkedList* aPlatforms);
	~WizardEntity(void);
	void pain(int hurt);
	void reset();
	bool checkForCollision(Entity* otherEntity);
	void drawSelf();
};

#endif
