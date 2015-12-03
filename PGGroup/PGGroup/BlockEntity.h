#ifndef _BLOCKENTITY_
#define _BLOCKENTITY_

#include "PlaneEntity.h"

class LinkedList;
class BlockEntity : public PlaneEntity {
private:
	LinkedList* planes;
	void createAndAdd(float x, float y, float z, GLfloat* vertices, Orientation orientation);

public:
	BlockEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, Orientation aOrientation, float widthX, float heightY, float lengthZ);
	~BlockEntity(void);
	bool hasCollided(Entity* otherEntity);
	bool isMovingToward(Entity* otherEntity);
	bool checkForCollision(Entity* otherEntity);
	void drawSelf();
};

#endif
