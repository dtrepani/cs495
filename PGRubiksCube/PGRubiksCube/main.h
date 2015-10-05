/*
* Name:			Desiree Trepanier
* Project:		PG2-3 - Rubiks Cube
* Description:	Textured rubik's cube using OpenGL that can be rotated and whose faces can be rotated like that
*				of an actual rubik's cube.
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
#include "Face.h"
using namespace std;

#define PI 3.1415926535897932384626433832795
#define WIDTH 640
#define HEIGHT 480
#define X 0
#define Y 1
#define Z 2

int main(int argc, char **argv);
void init();
void initFaces();
void initWindow();
void initOpenGL();
void initTextures();
void createTexture(int index);
void pollEventsAndDraw();
void checkKeys(SDL_Event &event, bool &running);
void checkKeysForCubeRotation(SDL_Event &event);
void checkKeysForSideRotation(SDL_Event &event);
void rotateCube();
void rotateToFace(int faceNum);
void drawRubiksCube();
void drawFace(int faceNum);

#endif
