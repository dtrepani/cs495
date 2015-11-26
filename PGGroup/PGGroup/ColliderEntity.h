#ifndef _COLLIDERENTITY_
#define _COLLIDERENTITY_

#include "Entity.h"

class ColliderEntity : public Entity {
private:
	ThreeAxis* entityPosition; // Used to always reference the colliders at a relative position.
	float radius;

public:
	ColliderEntity(ThreeAxis* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius, ThreeAxis *anEntityPosition);
	~ColliderEntity(void);
	float getRadius();
	ThreeAxis* getPosition();
};

#endif
