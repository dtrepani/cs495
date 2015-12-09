#ifndef _PLAYERENTITY_
#define _PLAYERENTITY_

#include "Entity.h"

#define Y_DEATH -10.0f

class InteractableEntity;
class LinkedList;
class PlayerEntity : public Entity {
private:
	typedef enum {
		STANDING,
		RUNNING,
		JUMPING,
		FALLING,
		DEAD
	} Status;

	Status state;
	GLint initialJumpTime, initialTurnTime;
	int health;
	float initialTurnDegree;
	InteractableEntity* interact;
	bool isTurning;

	void checkJump();
	void checkTurn180();

public:
	PlayerEntity(Vector* aPosition, float aRadius);
	~PlayerEntity(void);
	void pain(int hurt);
	void interactWith();
	void setInteract(InteractableEntity* src, InteractableEntity* anInteractableEntity);
	void jump();
	void turn180();
	void drawSelf(GLfloat (&matrix)[16], LinkedList* entities);
};

#endif
