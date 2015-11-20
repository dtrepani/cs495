#ifndef _COLLIDERENTITY_
#define _COLLIDERENTITY_

#include "Entity.h"

class ColliderEntity : public Entity {
private:
	int radius;

public:
	ColliderEntity(int aRadius, float posX, float posY, float posZ, GLuint *aTexture, GLuint aVBO);
	~ColliderEntity(void);
};

#endif
