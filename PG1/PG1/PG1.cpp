/*
* Name:			Desiree Trepanier
* Project:		PG1 - Pacman
* Description:	Ghostless and wall-less version of Pacman where the objective is to just to eat randomly positioned
*				dots using the arrow keys and play beautiful waka waka sounds. All the fun of Pacman without any of 
*				the pesky challenges. Also, his mouth moves.
*/
#include "PG1.h"

int main(int argc, char **argv) {
	init();
	pollEvents();
	close();
	return 0;
}

// The typical object initalizer "new SDL_Rect" is not available so we make our own.
SDL_Rect * newSDL_Rect(int x, int y, int w, int h) {
	SDL_Rect *rect = new SDL_Rect;
	rect->x = x; rect->y = y; rect->w = w; rect->h = h;
	return rect;
}

// Initialize variables, SDL, the window, the surface, the audio, and Pacman.
void init() {
	pmX = pmY = 0;
	pmWidth = pmHeight = pmOrigWidth = pmOrigHeight = 32;
	dotWH = 16;
	numDots = NUM_DOTS;
	startTime = SDL_GetTicks();
	walkDir = 3;

	srand (time(NULL));
	initDots();
	initSpritesheet();

	SDL_Init(SDL_INIT_VIDEO);
	mainWindow = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	mainSurface = SDL_GetWindowSurface(mainWindow);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	pmWaka = Mix_LoadWAV("media/waka.wav");

	imgSurface = IMG_Load("media\\pm_any_2.png");
	if (!imgSurface) printf("IMG_Load: %s\n", IMG_GetError());
	dotSurface = IMG_Load("media\\dot.png");
	if (!dotSurface) printf("IMG_Load: %s\n", IMG_GetError());

}

// Randomly place the dots throughout the screen.
void initDots() {
	for(int i = 0; i < numDots; i++) 
		dotRect[i] = newSDL_Rect(rand()%(640-dotWH), rand()%(480-dotWH), dotWH, dotWH);
}

// Used when animating Pacman as he moves. He has 3 keyframes total, but one keyframe is the same for every direction.
// Names refer to their images in the media subdirectory.
void initSpritesheet() {
	pmSprite[0][0] = "pm_up_0";
	pmSprite[0][1] = "pm_up_1";
	pmSprite[1][0] = "pm_down_0";
	pmSprite[1][1] = "pm_down_1";
	pmSprite[2][0] = "pm_left_0";
	pmSprite[2][1] = "pm_left_1";
	pmSprite[3][0] = "pm_right_0";
	pmSprite[3][1] = "pm_right_1";
}

// Uninitalize before closing the program.
void close() {
	SDL_FreeSurface(dotSurface);
	SDL_FreeSurface(imgSurface);
	SDL_DestroyWindow(mainWindow);
	Mix_Quit();
	SDL_Quit();
}

// Checks if the window close button has been clicked or any of the arrow keys pressed and reacts accordingly.
// Clicking the window close button breaks out of the loop in order to unitialize objects before the program exits.
// Arrow key presses will cause Pacman to move around the screen.
void pollEvents() {
	SDL_Event event;

	while(true) {
		if(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT)
				break;
			if(event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_UP:
						moveAndPlay("pmY", false);
						break;
					case SDLK_DOWN:
						moveAndPlay("pmY", true);
						break;
					case SDLK_LEFT:
						moveAndPlay("pmX", false);
						break;
					case SDLK_RIGHT:
						moveAndPlay("pmX", true);
						break;
				}
			}
		}

		currentKeyframe = ((SDL_GetTicks() - startTime) * 12) / 1000 % 3;

		imgSurface = IMG_Load(("media\\" + ((currentKeyframe == 2) ? "pm_any_2" : pmSprite[walkDir][currentKeyframe]) + ".png").c_str());
		if (!imgSurface) printf("IMG_Load: %s\n", IMG_GetError());

		redraw();
	}
}

// Move Pacman and animate his mouth as he moves. 
// walkDir is used to determine the proper image to load to the screen.
void moveAndPlay(string direction, bool add) {
	int addAmt = add ? 10 : -10;

	if (direction == "pmX") {
		walkDir = add ? 3 : 2;
		pmX += addAmt;
	}
	else if (direction == "pmY") {
		walkDir = add ? 1 : 0;
		pmY += addAmt;
	}

	checkForCollision();
}

// Check for collisions between Pacman and any dots. Play a sound and remove the dot on collision.
// A collision occurs if the distance between the center of Pacman and a dot is less than or equal to the sum of their radii.
void checkForCollision() {
	for(int i = 0; i < numDots; i++)
		if(distSquared(pmX + pmHeight/2, dotRect[i]->x + dotWH/2, pmY + pmHeight/2, dotRect[i]->y + dotWH/2) <= 0.8*(double)pow((double)pmHeight/2.0 + (double)dotWH/2.0, 2)) {
			Mix_PlayChannel(-1, pmWaka, 0);

			for(int j = i; j < numDots; j++)
				dotRect[i] = dotRect[j];

			numDots--;
		}
}

// Get the squared distance from the center of Pacman to the center of a dot.
double distSquared(int x1, int x2, int y1, int y2) {
	return ((double)pow((double)(x1-x2), 2) + (double)pow((double)(y1-y2), 2));
}

// Whenever anything changes (i.e., Pacman moves), the surface needs to be redrawn for the window to update.
void redraw() {
	SDL_FillRect(mainSurface, NULL, SDL_MapRGB(mainSurface->format,0,0,0));
	
	drawDots();
	movePacmanToOtherSideIfNeeded();
	loopPacmanOnEdges();

	SDL_UpdateWindowSurface(mainWindow);
}

// Draw the dots on the screen.
void drawDots() {
	int errorCheck;

	for(int i = 0; i < numDots; i++) {
		errorCheck = SDL_BlitSurface(dotSurface, NULL, mainSurface, dotRect[i]);
		if(errorCheck < 0) printf("SDL_BlitSurface: %s\n", SDL_GetError());
	}
}

// If Pacman is outside the window, move him to the other side of the window to keep moving.
void movePacmanToOtherSideIfNeeded() {
	while (pmX < 0) pmX += 640;
	while (pmX >= 640) pmX = 0;
	while (pmY < 0) pmY += 480;
	while (pmY >= 480) pmY = 0;
}


// If Pacman is partially off the edge of the window, the part of him that would be outside the
// window will be drawn on the other side.
void loopPacmanOnEdges() {
	SDL_Rect *imgRect;
	int errorCheck;

	for(int i = -1; i <= 1; i++)
		for(int j = -1; j <= 1; j++) {
			delete mainRect;
			mainRect = scaledIntersection(0, 0, 640, 480,
										  pmX + i*640, pmY + j*480, pmWidth, pmHeight,
										  1, 1);
			imgRect = scaledIntersection(0, 0, pmWidth, pmHeight,
										 -(pmX + i*640), -(pmY + j*480), 640, 480,
										 pmOrigWidth/pmWidth, pmOrigHeight/pmHeight);
			errorCheck = SDL_BlitScaled(imgSurface, imgRect, mainSurface, mainRect);
			if(errorCheck < 0) printf("SDL_BlitScaled: %s\n", SDL_GetError());
			delete imgRect;
		}
}

/* Used to draw partial parts of Pacman on the edge of the screen.
*  Determines the location and size of the intersection of the screen and Pacman using the largest value of the upper-left corner of the rectangles
*  (using x and y) to determine the upper-left corners of the new rectangle and the smallest value of the lower-right corners 
*  (using x+width and y+height) for the lower-right corners of the new rectangle.
*  Returns the intersection, which will be used to draw the proper parts of the image to the screen. */
SDL_Rect * scaledIntersection(int r1X, int r1Y, int r1Width, int r1Height,
							  int r2X, int r2Y, int r2Width, int r2Height,
							  double scaleX, double scaleY) {
	int newX, newY, newWidth, newHeight,
		ulX, ulY, // upper-left corners of intersection
		lrX, lrY; // lower-right corners of intersection

	ulX = MAX(r1X, r2X);
	ulY = MAX(r1Y, r2Y);
	lrX = MIN(r1X + r1Width, r2X + r2Width);
	lrY = MIN(r1Y + r1Height, r2Y + r2Height);

	newX = (int)(ulX * scaleX);
	newY = (int)(ulY * scaleY);
	newWidth = (int)((lrX - ulX) * scaleX);
	newHeight = (int)((lrY - ulY) * scaleY);

	return newSDL_Rect(newX, newY, newWidth, newHeight);
}
