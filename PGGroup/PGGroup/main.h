/*
* Name:			
* Project:		PGGroup
* Description:	
*/
#ifndef _MAIN_
#define _MAIN_

#include <cmath>
#include "Entity.h"
#include "PlaneEntity.h"
#include "BlockEntity.h"
#include "InteractableEntity.h"
#include "PlayerEntity.h"
#include "WizardEntity.h"
#include "glQuaternion.h"
using namespace std;

#define HEIGHT 600
#define WIDTH 800

// Load entites and texture on per-level basis
// REMEMBER: Will need a list of all entities per level (or grid of lists of entities based on position?)
//			And a separate list for the level's planes (walls, floors, etc)
// Below methods better suited in a level superclass
#include "LinkedList.h"
Entity* createEntity(string name, GLuint* texture, GLfloat* vertices, float radius, float x, float y, float z);
PlaneEntity* createPlaneEntity(string name, GLuint* texture, Orientation orientation, GLfloat* vertices, float x, float y, float z);
BlockEntity* createBlockEntity(string name, GLuint* texture, float x, float y, float z, float widthX, float heightY, float lengthZ);
InteractableEntity* createInteractableEntity(string name, GLuint* texture, GLfloat* vertices, float radius, float x, float y, float z);
PlayerEntity* createPlayerEntity(float x, float y, float z, float radius);
WizardEntity* createWizardEntity(string name, GLfloat* vertices, float radius, float x, float y, float z);

int main(int argc, char **argv);
void init();
void initWindow();
void initOpenGL();
void pollEventsAndDraw();
GLuint* createTexture(string name);
void movePlayer(bool* keys, PlayerEntity* player);
void printFPS(GLint &frames, GLint &previousTime);

#endif
