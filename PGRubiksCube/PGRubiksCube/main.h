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
using namespace std;

#define PI 3.1415926535897932384626433832795
#define WIDTH 640
#define HEIGHT 480
#define X 0
#define Y 1
#define Z 2

typedef struct {
	int top;
	int bottom;
	int left;
	int right;
} Face;

typedef struct {
	int origTexture;
	int texCol;
	int texRow;
	int angle;
} Quad;

int main(int argc, char **argv);
void init();
void initFaces();
void initWindow();
void initOpenGL();
void initDisplayList();
void initTextures();
void createTexture(int index);
void pollEventsAndDraw();
void checkKeys(SDL_Event &event, bool &running);
void checkKeysForCubeRotation(SDL_Event &event);
void checkKeysForSideRotation(SDL_Event &event);
void rotateCube();
void rotateToFace(int face);
void rotateSide(int face);
void rotateFaceXToFaceY(int srcFace, string faceX, string faceY);
void findWhichQuadsToRotate(string adjFace, int &col, int &row, bool &colConstant);
int getAdjFaceFromString(int face, string adjFace);
void getQuadsForFace(int face, Quad (&quadFace)[3][3]);
int getColForNum(int num);
int getRowForNum(int num);
void glPopAndPushMatrix();
void drawRubiksCube();
void drawQuadsOnCube();
void drawQuadsOnFace(int face, double increment);
void drawCubeFace(int face);
void drawQuad(int col, int row);

#endif
