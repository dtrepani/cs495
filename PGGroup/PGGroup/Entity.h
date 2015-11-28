#ifndef _ENTITY_
#define _ENTITY_

#include <iostream>
#include <string>
#include <cstdlib>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Vector.h"
using namespace std;

#define SENSITIVITY 0.2f

typedef enum {
	POSITION,
	ROTATION,
	VELOCITY
} LocationInfo;

class PlaneEntity;
class ColliderEntity;
class ColliderLinkedList;
class Entity {
protected:
	Vector* position;
	Vector* rotation;
	Vector* velocity;
	float opacity;
	GLuint* texture;
	GLfloat* vertices;
	ColliderLinkedList* colliders; // REMEMBER: Colliders should be relative to the position of the entity
	
	Vector* getCorrespondingVector(LocationInfo locationInfo);
	void rotateEntity();

public:
	Entity(Vector* aPosition, GLuint *aTexture, GLfloat* aVertices);
	~Entity(void);

	virtual bool hasCollided(Entity* otherEntity);
	virtual void checkForCollision(Entity* otherEntity);
	virtual bool isMovingToward(Entity* otherEntity);
	virtual void addCollider(float x, float y, float z, float radius);
	virtual void drawSelf();

	void move(float gravity);
	bool hasCollided(PlaneEntity* otherEntity);
	void checkForCollision(PlaneEntity* otherEntity);
	bool isMovingToward(PlaneEntity* otherEntity);
	void incrementXOf(LocationInfo locInfo, float x);
	void incrementYOf(LocationInfo locInfo, float y);
	void incrementZOf(LocationInfo locInfo, float z);
	Vector* getPosition();
	Vector* getRotation();
	Vector* getVelocity();
	ColliderLinkedList* getColliders();
};

#endif
