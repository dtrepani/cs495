#ifndef _PLAYERENTITY_
#define _PLAYERENTITY_

#include "Entity.h"

#define Y_DEATH -10.0f

class CoinInteractableEntity;
class InteractableEntity;
class LinkedList;
class PlaneEntity;
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
	GLint initialJumpTime, initialTurnTime, initialGumballTime;
	int health;
	float initialTurnDegree;
	bool isTurning, pickedUpCoin;
	bool shipDestroyed; // For when the wizard blows apart the ship; player will check for collisions with the platforms then
	InteractableEntity* interact;
	CoinInteractableEntity* coin;
	LinkedList* wizardSpawned; // Entities the wizard spawns that the player will need to check for collisions on
	PlaneEntity* gumball;

	void checkJump();
	void checkTurn180();
	void resetPos();
	void animateGumball();

public:
	PlayerEntity(Vector* aPosition, float aRadius);
	~PlayerEntity(void);
	void pain(int hurt);
	void addWizardSpawnedEntity(Entity* entity);
	void addCoin(CoinInteractableEntity* aCoin);
	void interactWith();
	void setInteract(InteractableEntity* src, InteractableEntity* anInteractableEntity);
	void jump();
	void turn180();
	void drawSelf(GLfloat (&matrix)[16], LinkedList* entities, LinkedList* platforms);

	bool isDead();
	bool hasPickedUpCoin();
	void pickUpCoin();
	void showGumball(bool show);
};

#endif
