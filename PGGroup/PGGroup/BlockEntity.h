#ifndef _BLOCKENTITY_
#define _BLOCKENTITY_

#include "PlaneEntity.h"
#include "LinkedList.h"

class BlockEntity : public PlaneEntity {
protected:
	LinkedList* planes;
	virtual void createAndAdd(float x, float y, float z, GLfloat* vertices, Orientation orientation);

public:
	BlockEntity(Vector* aPosition, GLuint *aTexture, float widthX, float heightY, float lengthZ);
	~BlockEntity(void);
	bool hasCollided(Entity* otherEntity);
	bool isMovingToward(Entity* otherEntity);
	bool checkForCollision(Entity* otherEntity);
	void drawSelf();
};

#endif
