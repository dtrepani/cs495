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
// Below methods better suited in a level superclass
Entity* createEntity(string name, GLuint* texture, GLfloat* vertices, float x, float y, float z);
InteractableEntity* createInteractableEntity(string name, GLuint* texture, GLfloat* vertices, float x, float y, float z);
PlayerEntity* createPlayerEntity(float x, float y, float z);
WizardEntity* createWizardEntity(string name, GLfloat* vertices, float x, float y, float z);

int main(int argc, char **argv);
void init();
void initWindow();
void initOpenGL();
void pollEventsAndDraw();
GLuint* createTexture(string name);

#endif
