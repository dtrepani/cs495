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
	Vector* pointOnPlane;
	float getRandValOnPlane(int axis);
	bool entityWithinPlaneBoundaries(Vector* otherPosition);

public:
	PlaneEntity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, Orientation aOrientation);
	~PlaneEntity(void);
	float getMin(int axis);
	float getMax(int axis);
	bool hasCollided(Entity* otherEntity);
	bool isMovingToward(Entity* otherEntity);
	bool checkForCollision(Entity* otherEntity);
	Orientation getOrientation();
};

#endif
