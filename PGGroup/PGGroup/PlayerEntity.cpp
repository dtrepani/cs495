#include "PlayerEntity.h"
#include "ColliderEntity.h"
#include "ColliderLinkedList.h"

#define PI 3.1415926535897932384626433832795

PlayerEntity::PlayerEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices)
: Entity(aPosition, aTexture, aVertices) {
	state = STANDING;
	isTurning = false;
	interact = false;
	health = 100; // can change later if wanted ?
	addCollider(0, 0, 0, 0);
	sensitivityRotation = 1.5f;
}

PlayerEntity::~PlayerEntity(void) {}

void PlayerEntity::pain(int hurt){
	health -= hurt;
	if (health <= 0)
		state = DEAD;
}

void PlayerEntity::toggleInteract(){
	interact = !interact;
}

// Player jumps.
// The animation will be gradual and take place over ~200ms, so the initial time that space was pressed must be recorded.
void PlayerEntity::jump(){
	if(state != JUMPING) {
		state = JUMPING;
		initialJumpTime = SDL_GetTicks();
	}
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

// Add a collider entity to the list of colliders
void PlayerEntity::addCollider(float x, float y, float z, float radius) { colliders->add(new ColliderEntity(new Vector(x, y, z), NULL, NULL, radius, position, true)); }

// Player moves forwards or backwards based on the direction they're currently facing.
void PlayerEntity::moveForward(bool forward) {
	float sensitivity = forward ? -SENSITIVITY : SENSITIVITY;
	float yaw = rotation->getY() * (PI / 180);
	//float pitch = rotation->getX() * (PI / 180);
	
	velocity->setX( -sin(yaw) * sensitivity );
	//position->incrementY( sin(pitch) * sensitivity );
	velocity->setZ( cos(yaw) * sensitivity );
}

// Player strafes left or right based on the direction they're currently facing.
void PlayerEntity::strafe(bool left) {
	float sensitivity = left ? -SENSITIVITY : SENSITIVITY;
	float yaw = rotation->getY() * (PI / 180);
	
	velocity->setX( cos(yaw) * sensitivity/2 );
	velocity->setZ( sin(yaw) * sensitivity/2 );
}

// Rotates player left or right
void PlayerEntity::rotate(bool left) {
	incrementYOf(ROTATION, left ? -sensitivityRotation : sensitivityRotation);
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

	if(entities->checkForCollision(this)) {
		state = STANDING;
	}

	Vector* tmp = position;
	position = position->add(velocity);
	velocity->zero();
	delete tmp;

	glRotatef( rotation->getY(), 0, 1, 0 );
	glTranslatef(-position->getX(), -position->getY()-0.3f, -position->getZ()); 

	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}
