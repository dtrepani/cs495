#ifndef _PLANEENTITY_
#define _PLANEENTITY_

#include "Entity.h"

typedef enum {
	BLOCK,
	VERTICAL_X,
	VERTICAL_Z,
	HORIZONTAL
} Orientation;

class PlaneEntity : public Entity {
private:
	Orientation orientation;

	bool entityWithinPlaneBoundaries(Vector* otherPosition);

public:
	PlaneEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, Orientation aOrientation);
	~PlaneEntity(void);
	float getSmallestPositionValFor(int axis);
	float getBiggestPositionValFor(int axis);
	bool hasCollided(Entity* otherEntity);
	bool isMovingToward(Entity* otherEntity);
	bool checkForCollision(Entity* otherEntity);
	Orientation getOrientation();
};

#endif
