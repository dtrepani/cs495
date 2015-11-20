#ifndef _ENTITY_
#define _ENTITY_

#include <iostream>
#include <string>
#include <cstdlib>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "LinkedList.h"
#include "ThreeAxis.h"
using namespace std;

class ThreeAxis;
class LinkedList;
class Entity {
private:

protected:
	ThreeAxis* position;
	ThreeAxis* rotation;
	ThreeAxis* velocity;
	float opacity;
	GLuint* texture;
	GLuint vbo;//vertex buffer object?
	LinkedList* colliders;// REMEMBER: Colliders should be relative to the position of the entity
	
	bool hasCollided(LinkedList* otherColliders);

public:
	Entity(float posX, float posY, float posZ,
			GLuint *aTexture, GLuint aVBO);
	~Entity(void);
	void move(float gravity);

	ThreeAxis* getPosition();
	LinkedList* getColliders();
};

#endif
