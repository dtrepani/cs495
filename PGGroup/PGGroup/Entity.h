#ifndef _ENTITY_
#define _ENTITY_

#include <iostream>
#include <string>
#include <cstdlib>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Vector.h"
#include "glQuaternion.h"
using namespace std;

#define X 0
#define Y 1
#define Z 2
#define SENSITIVITY 0.2f
#define SENSITIVITY_ROTATION 1.5f

typedef enum {
	POSITION,
	ROTATION,
	VELOCITY,
	SCALE
} LocationInfo;

class PlaneEntity;
class Entity {
protected:
	Vector* position;
	Vector* rotation;
	Vector* velocity;
	Vector* scale;
	float	opacity, radius;
	bool	passable;
	GLuint* texture;
	GLfloat vertices[12];
	Entity* parent;
	
	Vector* getCorrespondingVector(LocationInfo locationInfo);
	void rotateEntity();
	void addVelocityToPosition();

public:
	Entity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices, float aRadius);
	~Entity(void);

	virtual bool hasCollided(Entity* otherEntity);
	virtual bool checkForCollision(Entity* otherEntity);
	virtual bool isMovingToward(Entity* otherEntity);
	virtual void drawSelf();

	void moveForward(bool forward);
	void strafe(bool left);
	void rotate(bool left);
	void setRandomRotation();
	void set(LocationInfo locInfo, float x, float y, float z);
	void incrementXOf(LocationInfo locInfo, float x);
	void incrementYOf(LocationInfo locInfo, float y);
	void incrementZOf(LocationInfo locInfo, float z);
	Vector* getPosition();
	Vector* getRotation();
	Vector* getVelocity();
	Vector* getScale();
	float getRadius();
	void setParent(Entity* aParent);
};

#endif
