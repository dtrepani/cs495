#ifndef _ENTITY_
#define _ENTITY_

#include <iostream>
#include <string>
#include <cstdlib>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ColliderLinkedList.h"
#include "ThreeAxis.h"
using namespace std;

class ThreeAxis;
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

public:
	Entity(ThreeAxis* aPosition, GLuint *aTexture, GLuint aVBO);
	~Entity(void);
	void move(float gravity);

	ThreeAxis* getPosition();
	ColliderLinkedList* getColliders();
};

#endif
