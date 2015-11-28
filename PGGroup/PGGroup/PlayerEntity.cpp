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
	// TO-DO: Enable jumping only when not jumping
	//		  Smooth jumping (increment over time instead of set)
	//if(state != JUMPING) {
		state = JUMPING;
		velocity->setY(3.0f);
	//}
}

// Add a collider entity to the list of colliders
void PlayerEntity::addCollider(float x, float y, float z, float radius) { colliders->add(new ColliderEntity(new Vector(x, y, z), NULL, NULL, radius, position, true)); }

// Player moves forwards or backwards based on the direction they're currently facing.
void PlayerEntity::moveForward(bool forward) {
	float sensitivity = forward ? -SENSITIVITY : SENSITIVITY;
	float yaw = rotation->getY() * (PI / 180);
	//float pitch = rotation->getX() * (PI / 180);
	
	velocity->setX( sin(yaw) * sensitivity );
	//position->incrementY( sin(pitch) * sensitivity );
	velocity->setZ( cos(yaw) * sensitivity );
}

void PlayerEntity::strafe(bool left) { // TO-DO: not working correctly
	float sensitivity = left ? -SENSITIVITY : SENSITIVITY;
	float yaw = rotation->getY() * (PI / 180);
	
	velocity->setX( cos(yaw) * sensitivity );
	velocity->setZ( sin(yaw) * sensitivity );
}

// Adjusts the camera to the player's position and rotation.
// Translation is negative because player's position is opposite what other entities' would be due to the rotation above.
void PlayerEntity::drawSelf(GLfloat (&matrix)[16]) {
	glLoadMatrixf(matrix);

	Vector* tmp = position;
	position = position->add(velocity);
	velocity->zero();
	delete tmp;

	glRotatef( 360.0f - rotation->getY(), 0, 1, 0 );
	glTranslatef(-position->getX(), -position->getY(), -position->getZ()); 

	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}
