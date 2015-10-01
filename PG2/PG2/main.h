/*
* Name:			Desiree Trepanier
* Project:		PG2-3 - Rubix Cube
* Description:	Textured rubix cube using OpenGL that can be rotated.
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

int main(int argc, char **argv);
void init();
void initWindow();
void initOpenGL();
void initTextures();
void createTexture(int index);
void pollEventsAndDraw();
void checkKeys(SDL_Event &event, bool &running);
void setRotation(int axis, bool posMovement, bool negMovement);
void drawRubixCube();
void rotateCube();
double getRotationAmount(int axis);
void drawQuad(int face, double offset);
float getVertex(int faceIndex, int vertexIndex, double offset);

#endif
