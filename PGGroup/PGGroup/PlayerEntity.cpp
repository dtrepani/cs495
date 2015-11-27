#include "PlayerEntity.h"
#include "ColliderEntity.h"
#include "ColliderLinkedList.h"

#define PI 3.1415926535897932384626433832795

PlayerEntity::PlayerEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices)
: Entity(aPosition, aTexture, aVertices) {
	state = STANDING;
	interact = false;
	health = 100; // can change later if wanted ?
	addCollider(0, 0, 0, 0);

	sensitivityMovement = 0.2f;
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

void PlayerEntity::jump(){
	state = JUMPING;
	//velocity.y = NEED TO FIND VARIABLE THAT WORKS SMOOTHLY
}

// Add a collider entity to the list of colliders
void PlayerEntity::addCollider(float x, float y, float z, float radius) { colliders->add(new ColliderEntity(new Vector(x, y, z), NULL, NULL, radius, position, true)); }

void PlayerEntity::moveForward(bool forward) {
	float sensitivity = forward ? -sensitivityMovement : sensitivityMovement;
	float yaw = rotation->getY() * (PI / 180);
	float pitch = rotation->getX() * (PI / 180);
	
	position->incrementX( sin(yaw) * sensitivity );
	//position->incrementY( sin(pitch) * sensitivity );
	position->incrementZ( cos(yaw) * sensitivity );
}

void PlayerEntity::moveSideways(bool left) { // TO-DO: not working correctly (still moves on world x-axis)
	float sensitivity = left ? -sensitivityMovement : sensitivityMovement;
	float yaw = rotation->getY() * (PI / 180);
	
	position->incrementX( cos(yaw) * sensitivity );
	position->incrementZ( sin(yaw) * sensitivity );
}

// Adjusts the camera to the player's position and rotation.
void PlayerEntity::drawSelf(GLfloat (&matrix)[16]) {
	glLoadMatrixf(matrix);
	
	glRotatef( 360.0f - rotation->getY(), 0, 1, 0 );
	glTranslatef(-position->getX(), -position->getY(), -position->getZ()); // Negative because player's position is opposite what other entities' would be due to the rotation above.

	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}
