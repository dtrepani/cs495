/*
* Name:			Desiree Trepanier
* Project:		PG1 - Pacman
* Description:	Ghostless and wall-less version of Pacman where the objective is to just to eat randomly positioned
*				dots using the arrow keys and play beautiful waka waka sounds. All the fun of Pacman without any of 
*				the pesky challenges. Also, his mouth moves.
*/
#ifndef _PG1_
#define _PG1_

#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
using namespace std;

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define NUM_DOTS 30

int pmX, pmY, pmWidth, pmHeight, pmOrigWidth, pmOrigHeight, dotWH, currentKeyframe, numDots, walkDir, startTime; // pm = Pacman
string pmSprite[4][2];

SDL_Window *mainWindow;
SDL_Surface *mainSurface, *imgSurface, *dotSurface;
SDL_Rect *mainRect;
SDL_Rect *dotRect[NUM_DOTS];
Mix_Chunk *pmWaka;

int main(int argc, char **argv);
SDL_Rect * newSDL_Rect(int x, int y, int w, int h);
void init();
void initDots();
void initSpritesheet();
void close();
void pollEvents();
void moveAndPlay(string direction, bool add);
double distSquared(int x1, int x2, int y1, int y2);
void checkForCollision();
void redraw();
void drawDots();
void movePacmanToOtherSideIfNeeded();
void loopPacmanOnEdges();
SDL_Rect * scaledIntersection(int r1X, int r1Y, int r1Width, int r1Height,
							  int r2X, int r2Y, int r2Width, int r2Height,
							  double scaleX, double scaleY);

#endif
