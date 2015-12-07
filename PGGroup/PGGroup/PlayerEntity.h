#ifndef _PLAYERENTITY_
#define _PLAYERENTITY_

#include "Entity.h"

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
	bool interact;
	bool isTurning;
	float sensitivityRotation;

public:
	PlayerEntity(Vector* aPosition, float aRadius);
	~PlayerEntity(void);
	void pain(int hurt);
	void toggleInteract();
	void jump();
	void turn180();
	void moveForward(bool forward);
	void strafe(bool left);
	void rotate(bool left);
	void checkJump();
	void checkTurn180();
	void drawSelf(GLfloat (&matrix)[16], LinkedList* entities);
};

#endif
