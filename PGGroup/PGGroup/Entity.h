#ifndef _ENTITY_
#define _ENTITY_

#include <iostream>
#include <string>
#include <cstdlib>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ThreeAxis.h"
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
	ThreeAxis* position;
	ThreeAxis* rotation;
	ThreeAxis* velocity;
	float opacity;
	GLuint* texture;
	GLuint vbo;//vertex buffer object?
	ColliderLinkedList* colliders; // REMEMBER: Colliders should be relative to the position of the entity
	
	bool hasCollided(ColliderLinkedList* otherColliders);
	ThreeAxis* getCorrespondingThreeAxis(LocationInfo locationInfo);

public:
	Entity(ThreeAxis* aPosition, GLuint *aTexture, GLuint aVBO);
	~Entity(void);
	void move(float gravity);
	void addCollider(ColliderEntity* colliderEntity);

	void incrementXOf(LocationInfo aLocInfo, float x);
	void incrementYOf(LocationInfo aLocInfo, float y);
	void incrementZOf(LocationInfo aLocInfo, float z);
	ThreeAxis* getPosition();
	ColliderLinkedList* getColliders();
};

#endif
