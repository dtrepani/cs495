#ifndef _COLLIDERENTITY_
#define _COLLIDERENTITY_

#include "Entity.h"

class ColliderEntity : public Entity {
private:
	ThreeAxis* entityPosition; // Used to always reference the colliders at a relative position.
	int radius;

public:
	ColliderEntity(ThreeAxis* aPosition, GLuint *aTexture, GLfloat* aVertices, int aRadius, ThreeAxis *anEntityPosition);
	~ColliderEntity(void);
	int getRadius();
	ThreeAxis* getPosition();
};

#endif
