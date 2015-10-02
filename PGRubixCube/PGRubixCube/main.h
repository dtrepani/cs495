/*
* Name:			Desiree Trepanier
* Project:		PG2-3 - Rubix Cube
* Description:	Textured rubix cube using OpenGL that can be rotated and whose faces can be rotated like that
*				of an actual rubix cube.
*/
#ifndef _MAIN_
#define _MAIN_

#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
using namespace std;

#define PI 3.1415926535897932384626433832795
#define WIDTH 640
#define HEIGHT 480

typedef struct {
	bool posMovement;
	bool negMovement;
} AxisRotation;

/**************** TODO: Make a class for Quad. It REALLY needs it. ****************
	- Draw itself
	- Could return the texture col and row rather than continually passing around the face
*/
typedef struct {
	int origTexture;
	int texCol;
	int texRow;
	int angle;
} Quad;

int main(int argc, char **argv);
void init();
void initWindow();
void initOpenGL();
void initTextures();
void createTexture(int index);
void pollEventsAndDraw();
void checkKeys(SDL_Event &event, bool &running);
void setRotation(int axis, bool posMovement, bool negMovement);
void rotateCube();
double getRotationAmount(int axis);
void rotateToFace(int face);
void rotateFace(int face, bool clockwise);
void moveCorners(int face, bool clockwise);
void moveNonCorners(int face, bool clockwise);
int getColForNum(int num);
int getRowForNum(int num);
void glPopAndPushMatrix();
void drawRubixCube();
void drawQuadsOnCube();
void drawQuadsOnFace(int face, double increment);
void drawCubeFace(int face);
void drawQuad(int col, int row);

#endif
