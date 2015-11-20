/*
* Name:			
* Project:		PGGroup
* Description:	
*/
#ifndef _MAIN_
#define _MAIN_

#include <cmath>
#include "Entity.h"
#include "glQuaternion.h"
using namespace std;

#define HEIGHT 480
#define WIDTH 640

int main(int argc, char **argv);
void init();
void initWindow();
void initOpenGL();
void initTextures();
void createTexture(int index);

#endif
