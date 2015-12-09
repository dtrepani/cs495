#include "WizardEntity.h"

WizardEntity::WizardEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius)
: Entity(aPosition, aTexture, aVertices, aRadius) {
	state = FLOATING;
	health = 100;
	floatingAngle = 0;
	frame = 1;
	initialAnimTime = 0;
	passable = true;
}

WizardEntity::~WizardEntity(void) {}

// Reduce the wizard's health. Upon death, setup the death animation and fall to the floor.
void WizardEntity::pain(int hurt){
	health -= hurt;
	if (health <= 0) {
		state = DEAD;
		position->setY(0.9f);
		frame = 4;
		initialAnimTime = SDL_GetTicks();
	}
}

void WizardEntity::animate() {
	animFloat();
	animCast();
	animDeath();
}

// Animates the wizard floating.
void WizardEntity::animFloat() {
	if(state != DEAD) {
		velocity->setY( (float)sin( floatingAngle * (PI / 180) ) / 1000.0f );

		if(floatingAngle >= 359.0f)
			floatingAngle = 0;
		else
			floatingAngle += 1.0f;
	}
}

// Animates the wizard's "death".
// After 3 seconds, the wizard looks up at the player.
void WizardEntity::animDeath() {
	if((frame == 4) && timePassedGreaterThan(5000)) {
		frame++;
		initialAnimTime = 0;
	}
}

// Animes the wizard's cast.
// After casting, the wizard returns to floating.
void WizardEntity::animCast() {
	if((state == CASTING) && timePassedGreaterThan(1000)) {
		if(frame == 2 || frame == 3) {
			frame = (frame == 2) ? 3 : 1;
			initialAnimTime = SDL_GetTicks();
		} else {
			state = FLOATING;
			initialAnimTime = 0;
		}
	}
}

// If the initial animation time isn't set, no animation should be taking place.
bool WizardEntity::timePassedGreaterThan(int ms) {
	return ((initialAnimTime != 0) && ((SDL_GetTicks() - initialAnimTime) >= ms));
}

bool WizardEntity::checkForCollision(Entity* otherEntity) {
	bool collisionAndMovingToward = hasCollided(otherEntity) && isMovingToward(otherEntity);
	if(collisionAndMovingToward && (position->getY() <= otherEntity->getPosition()->getY())) {
		pain(10);
		// TO-DO: Player is always sent backwards in regards to where they're looking rather than backwards from the wizard
		for(int i = 0; i < 30; i++) {
			otherEntity->moveForward(false);
		}
	}
	return collisionAndMovingToward;
}

// The wizard needs to account for various animations (i.e., floating), thus needs his own version of the draw method.
void WizardEntity::drawSelf() {
	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0, opacity);
	glBindTexture( GL_TEXTURE_2D, *texture );
	
	animate();
	addVelocityToPosition();

	glTranslatef(position->getX(), position->getY(), position->getZ());
	rotateEntity();
	glScalef(scale->getX(), scale->getY(), scale->getZ());

	glBegin(GL_QUADS);
		glTexCoord2f(0.2f*(frame-1), 1.0f);		glVertex3f(vertices[0], vertices[1], vertices[2]);
		glTexCoord2f(0.2f*frame,	 1.0f);		glVertex3f(vertices[3], vertices[4], vertices[5]);
		glTexCoord2f(0.2f*frame,	 0);		glVertex3f(vertices[6], vertices[7], vertices[8]);
		glTexCoord2f(0.2f*(frame-1), 0);		glVertex3f(vertices[9], vertices[10], vertices[11]);
	glEnd();
	glPopMatrix();
}
