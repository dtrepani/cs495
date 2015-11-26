#ifndef _PLAYERENTITY_
#define _PLAYERENTITY_

#include "Entity.h"

class PlayerEntity : public Entity {
private:
	typedef enum {
		STANDING,
		RUNNING,
		JUMPING,
		DEAD
	} Status;

	int health;
	Status state;
	bool interact;
	float movementForwardBackward, movementSideways, movementRotation;

public:
	PlayerEntity(ThreeAxis* aPosition, GLuint *aTexture, GLfloat* aVertices);
	~PlayerEntity(void);
	void pain(int hurt);
	void toggleInteract();
	void jump();
	void moveForward(bool forward);
	void moveSideways(bool left);
	void drawSelf(GLfloat (&matrix)[16]);
};

#endif
