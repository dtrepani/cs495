#ifndef _ENTITY_
#define _ENTITY_

#include <iostream>
#include <string>
#include <cstdlib>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "LinkedList.h"
using namespace std;

class LinkedList;
class Entity {
private:
	typedef struct {
		float x, y, z;
	} ThreeAxis;

protected:
	ThreeAxis position,
			  rotation,
			  velocity;
	float opacity;
	GLuint *texture;
	GLuint vbo;//vertex buffer object?
	LinkedList *colliders;// REMEMBER: Colliders should be relative to the position of the entity	
	//virtual void idleAnimation(void) = 0;

public:
	//Entity();
	Entity(float posX, float posY, float posZ,
			GLuint *aTexture, GLuint aVBO);
	~Entity(void);
	void move(float gravity);
};

#endif
