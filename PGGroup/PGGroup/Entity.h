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

typedef enum {
	POSITION,
	ROTATION,
	VELOCITY
} LocationInfo;

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
	void move(float gravity);
	bool hasCollided(Entity* otherEntity);
	virtual void addCollider(float x, float y, float z, float radius);
	virtual void drawSelf();
	void incrementXOf(LocationInfo locInfo, float x);
	void incrementYOf(LocationInfo locInfo, float y);
	void incrementZOf(LocationInfo locInfo, float z);
	void setXYZOf(LocationInfo locInfo, float x, float y, float z);
	Vector* getPosition();
	Vector* getRotation();
	Vector* getVelocity();
	ColliderLinkedList* getColliders();
};

#endif
