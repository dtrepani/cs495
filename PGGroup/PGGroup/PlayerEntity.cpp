#include "PlayerEntity.h"

#define PI 3.1415926535897932384626433832795

PlayerEntity::PlayerEntity(ThreeAxis* aPosition, GLuint *aTexture, GLfloat* aVertices)
: Entity(aPosition, aTexture, aVertices) {
	state = STANDING;
	interact = false;
	health = 100; // can change later if wanted ?
	addCollider(0, 0, 0, 0);

	movementForwardBackward = 0.2f;
	movementSideways = 0.1f;
	movementRotation = 1.5f;
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

void PlayerEntity::jump(){
	state = JUMPING;
	//velocity.y = NEED TO FIND VARIABLE THAT WORKS SMOOTHLY
}

void PlayerEntity::moveForward(bool forward) {
	float movement	= forward ? movementForwardBackward : -movementForwardBackward;
	float yaw		= rotation->getY() * (PI / 180);
	
	position->incrementX( (float)sin(yaw) * movement );
	position->incrementZ( (float)cos(yaw) * movement );
}

void PlayerEntity::moveSideways(bool left) {
	float movement	= left ? movementSideways : -movementSideways;
	float pitch		= rotation->getX() * (PI / 180);
	float yaw		= rotation->getY() * (PI / 180);
	
	position->incrementX( (float)sin(pitch) * movement );
	position->incrementZ( (float)cos(pitch) * movement );
}

// Adjusts the camera to the player's position and rotation.
void PlayerEntity::drawSelf(GLfloat (&matrix)[16]) {
	glLoadMatrixf(matrix);
	
	glRotatef( 360.0f - rotation->getY(), 0, 1, 0 ); // TC
	glTranslatef(position->getX(), position->getY(), position->getZ());

	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}
