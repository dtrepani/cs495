#include "PlayerEntity.h"
#include "LinkedList.h"
#include "InteractableEntity.h"

PlayerEntity::PlayerEntity(Vector* aPosition, float aRadius)
: Entity(aPosition, NULL, NULL, aRadius) {
	state = STANDING;
	isTurning = false;
	passable = true;
	interact = NULL;
	health = 100;
}

PlayerEntity::~PlayerEntity(void) {}

void PlayerEntity::pain(int hurt){
	health -= hurt;
	if (health <= 0) { // TO-DO: enable actual death rather than just position reset
		//state = DEAD;
		position->setX(0);
		position->setY(1.0f);
		position->setZ(0);
		rotation->zero();
	}
}

void PlayerEntity::interactWith() {
	if(interact) interact->interactWith(this);
}

// The entity sending the interaction set must be sent as a parameter to ensure that only the intertactable entity the player
// is currently collided with can change the interact variable. This is necessary because an interactable entity will always
// call this when checking for collisions (will pass in NULL if no collision).
// Does not support stacked interactable collisions.
void PlayerEntity::setInteract(InteractableEntity* src, InteractableEntity* anInteractableEntity) {
	if(!interact || src == interact) {
		interact = anInteractableEntity;
	}
}

// Player jumps.
// The animation will be gradual and take place over ~200ms, so the initial time that space was pressed must be recorded.
void PlayerEntity::jump(){
	//if(state != JUMPING && state != FALLING) { // TO-DO: re-add condition when done testing
		state = JUMPING;
		initialJumpTime = SDL_GetTicks();
	//}
}

// Player rotates to the opposite direction. 
// The animation will be gradual and take place over 100ms and then set the player rotation to the exact opposite rotation, so
// the initial rotation and the time that x was pressed must be recorded.
void PlayerEntity::turn180(){
	if(!isTurning) {
		isTurning = true;
		initialTurnTime = SDL_GetTicks();
		initialTurnDegree = rotation->getY();
	}
}


// The animation for jumping, if it is occurring.
// Takes place over 200 ms and will set the player's state to falling when done.
void PlayerEntity::checkJump() {
	if(state == JUMPING) {
		if(SDL_GetTicks() - initialJumpTime < 200) {
			velocity->incrementY(0.5f);
		} else {
			state = FALLING;
		}
	}
}

// The animation for turning 180 degrees, if it is occurring.
// Takes place over 100 ms and will set the player's rotation to 180 degrees after the animation has occurred to ensure
// that the player's spin always lands them in the exact opposite direction.
void PlayerEntity::checkTurn180() {
	if(isTurning) {
		if(SDL_GetTicks() - initialTurnTime < 100) {
			rotation->incrementY(30.0f);
		} else {
			isTurning = false;
			rotation->setY(initialTurnDegree + 180.0f);
		}
	}
}

// Adjusts the camera to the player's position and rotation.
// Translation is negative because player's position is opposite what other entities' would be due to the rotation above.
// The Y translation is adjusted slightly to make the camera act as if it's out of the eyes of a person (off-center), while
// maintaining the actual center of the player.
void PlayerEntity::drawSelf(GLfloat (&matrix)[16], LinkedList* entities) {
	glLoadMatrixf(matrix);

	checkJump();
	checkTurn180();

	// Check for collisions and set the player's state accordingly.
	bool collision = entities->checkForCollision(this);
	if(state != JUMPING) {
		if(collision)
			state = STANDING;
		else
			state = FALLING;
	}

	addVelocityToPosition();

	//printf("(%f, %f, %f)\n", position->getX(), position->getY(), position->getZ());

	if(position->getY() < Y_DEATH) pain(999); // Player instantly dies if past the Y_DEATH point.

	glRotatef( rotation->getY(), 0, 1, 0 );
	glTranslatef(-position->getX(), -position->getY()-0.3f, -position->getZ()); 

	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}
