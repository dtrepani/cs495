#ifndef _PLANEENTITY_
#define _PLANEENTITY_

#include "Entity.h"

#define X 0
#define Y 1
#define Z 2

typedef enum {
	VERTICAL_X,
	VERTICAL_Z,
	HORIZONTAL
} Orientation;

class PlaneEntity : public Entity {
private:
	Orientation orientation;

	float getSmallestPositionValFor(int axis);
	float getBiggestPositionValFor(int axis);
	bool entityWithinPlaneBoundaries(Vector* otherPosition);

public:
	PlaneEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, Orientation aOrientation);
	~PlaneEntity(void);
	bool hasCollided(Entity* otherEntity);
	bool isMovingToward(Entity* otherEntity);
	void checkForCollision(Entity* otherEntity);
};

#endif
