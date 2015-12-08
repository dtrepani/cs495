#include "WizardEntity.h"

WizardEntity::WizardEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius)
: Entity(aPosition, aTexture, aVertices, aRadius) {
	state = FLOATING;
	health = 100;
	floatingAngle = 0;
}

WizardEntity::~WizardEntity(void) {}

void WizardEntity::pain(int hurt){
	health -= hurt;
	if (health <= 0)
		state = DEAD;
}

// Animates the wizard floating.
void WizardEntity::floating() {
	if(state == FLOATING) {
		velocity->setY( (float)sin( floatingAngle * (PI / 180) ) / 1000.0f );

		if(floatingAngle >= 359.0f)
			floatingAngle = 0;
		else
			floatingAngle += 1.0f;
	}
}

// The wizard needs to account for various animations (i.e., floating), thus needs his own version of the draw method.
void WizardEntity::drawSelf() {
	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0, opacity);
	glBindTexture( GL_TEXTURE_2D, *texture );
	
	floating();
	addVelocityToPosition();

	glTranslatef(position->getX(), position->getY(), position->getZ());
	rotateEntity();
	glScalef(scale->getX(), scale->getY(), scale->getZ());

	glBegin(GL_QUADS);
		glTexCoord2f(0, 1.0f);		glVertex3f(vertices[0], vertices[1], vertices[2]);
		glTexCoord2f(1.0f, 1.0f);	glVertex3f(vertices[3], vertices[4], vertices[5]);
		glTexCoord2f(1.0f, 0);		glVertex3f(vertices[6], vertices[7], vertices[8]);
		glTexCoord2f(0, 0);			glVertex3f(vertices[9], vertices[10], vertices[11]);
	glEnd();
	glPopMatrix();
}
