#include "WizardEntity.h"
#include "PlayerEntity.h"
#include "LinkedList.h"

WizardEntity::WizardEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius, PlayerEntity* aPlayer, LinkedList* aCovers, LinkedList* aPlatforms)
: Entity(aPosition, aTexture, aVertices, aRadius) {
	player = aPlayer;
	covers = aCovers;
	platforms = aPlatforms;
	
	animLocs[0] = new Vector(0,	1.5f,	-7.0f); // Spawn point
	animLocs[1] = new Vector(0,	1.5f,	-27.0f); // Before cast to destroy ship
	animLocs[2] = new Vector(0,	4.0f,	-27.0f); // Rising up as casting to destroy ship
	animLocs[3] = new Vector(0,	1.5f,	-82.0f); // Other side of destroyed gap
	animLocs[4] = new Vector(0,	1.5f,	-115.0f); // Before rising up to get away
	animLocs[5] = new Vector(0,	10.0f,	-115.0f); // Got away
	
	reset();
}

WizardEntity::~WizardEntity(void) {}

void WizardEntity::reset() {
	position->setEqualTo(animLocs[0]);
	state = FLOATING;
	health = 100;
	floatingAngle = 0;
	initialAnimTime = initialAnimLocTime = 0;
	animLocI = 1;
	frame = 1;
	passable = true;
	shipDestroyed = false;
}

// Reduce the wizard's health. Upon death, setup the death animation and fall to the floor.
void WizardEntity::pain(int hurt){
	health -= hurt;
	if (health <= 0 && state != DEAD) {
		state = DEAD;
		position->setY(1.1f);
		frame = 4;
		initialAnimTime = SDL_GetTicks();
	}
}

void WizardEntity::setCast() {
	state = CASTING;
	frame = 2;
	initialAnimTime = SDL_GetTicks();
}

void WizardEntity::animate() {
	animFloat();
	animCast();
	animDeath();
	animMovement();
}

// Animate the movement of the wizard throughout the wizard.
// The wizard will not move further if he is dead or if the player is more than 50.0f away (or 10.0f away if animLocI is 4 (waiting on other side of gap).
// He casts a spell to break the ship apart at at animLoc 1 and casts a normal spell at animLoc 3.
// He moves incredibly fast across the gap and slow-ish otherwise.
void WizardEntity::animMovement() {
	if( (state != DEAD) 
		&& ((SDL_GetTicks() - initialAnimLocTime) >= ((animLocI == 2) ? 500 : (animLocI == 3) ? 2000 : 1000)) 
		&& (position->distanceTo(player->getPosition()) < ((animLocI == 4) ? 10.0f : 50.0f))) {
		if(position->equalsWithError(animLocs[animLocI], 0.5f)) {
			if(animLocI == 1 || animLocI == 3) setCast();
			if(animLocI < 5) {
				animLocI++;
				initialAnimLocTime = SDL_GetTicks();
			}
		} else {
			velocity = ( ((animLocs[animLocI])->subtract(position))->normalize() )->scalarMultiply( (animLocI == 3) ? 1.0f : (animLocI == 4 || animLocI == 5) ? 0.08 : 0.1f);
		}
	}
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

// Animates the wizard's cast.
// After casting, the wizard returns to floating.
void WizardEntity::animCast() {
	if((state == CASTING) && timePassedGreaterThan(1000)) {
		if(frame == 2 || frame == 3) {
			if(frame == 3 && animLocI == 3) {
				shipDestroyed = true;
			}
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

// Cause pain to the wizard if player hits on the head and send player backward.
bool WizardEntity::checkForCollision(Entity* otherEntity) {
	bool collisionAndMovingToward = hasCollided(otherEntity) && isMovingToward(otherEntity);
	if(collisionAndMovingToward && (position->getY() <= otherEntity->getPosition()->getY())) {
			pain(10);
			for(int i = 0; i < 30; i++)
				otherEntity->moveForward(false);
	}
	return collisionAndMovingToward;
}

// The wizard needs to account for various animations (i.e., floating) and the destruction of the spaceship, thus needs his own version of the draw method.
void WizardEntity::drawSelf() {
	if(shipDestroyed) {
		platforms->drawSelf();
	} else {
		// Player can't pass wizard before the ship is destroyed
		if( abs(position->getZ() - player->getPosition()->getZ()) < 1.0f ) {
			for(int i = 0; i < 30; i++)
				player->moveForward(false);
		}
		covers->drawSelf();
	}

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
