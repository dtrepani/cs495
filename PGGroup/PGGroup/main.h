/*
* Name:			
* Project:		PGGroup
* Description:	
*/
#ifndef _MAIN_
#define _MAIN_

#include <cmath>
#include "Entity.h"
#include "InteractableEntity.h"
#include "PlayerEntity.h"
#include "WizardEntity.h"
#include "glQuaternion.h"
using namespace std;

#define HEIGHT 480
#define WIDTH 640

// Load entites and texture on per-level basis
// REMEMBER: Will need a list of all entities per level (or grid of lists of entities based on position?)
//			And a separate list for the level's planes (walls, floors, etc)

typedef enum { // better suited in the level superclass
	NORMAL,
	INTERACTABLE,
	PLAYER,
	WIZARD
} EntityType;

int main(int argc, char **argv);
void init();
void initWindow();
void initOpenGL();
void pollEventsAndDraw();
GLuint* createTexture(string name);
Entity* createModel(string name, GLfloat* vertices, EntityType entityType,
					float x, float y, float z);

#endif
