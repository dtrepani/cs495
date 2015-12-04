#ifndef _COLLIDERENTITY_
#define _COLLIDERENTITY_

#include "Entity.h"

class ColliderEntity : public Entity {
private:
	Vector* entityPosition; // Used to always reference the colliders at a relative position.
	float radius;
	bool isPlayer;

public:
	ColliderEntity(Vector* aPosition, GLuint* aTexture, GLfloat* aVertices, float aRadius, Vector* anEntityPosition, bool aIsPlayer);
	~ColliderEntity(void);
	float getRadius();
	Vector* getPosition();
};

#endif
