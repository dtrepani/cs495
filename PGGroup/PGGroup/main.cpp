/*
* Name:			
* Project:		PGGroup
* Description:	
*/
#include "main.h"

SDL_Window *mainWindow;
SDL_Surface **textureSurface;
GLuint *texture;
glQuaternion qAngle;

int main(int argc, char **argv) {
	init();
	// poll events and draw

	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
	return 0;
}

// Initialize variables and call initalizations for the window, openGL, and textures
void init() {
	initWindow();
	initOpenGL();
	initTextures();
}

// Initalize window
void initWindow() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	mainWindow = SDL_CreateWindow(	"Game",
									SDL_WINDOWPOS_UNDEFINED,
									SDL_WINDOWPOS_UNDEFINED,
									WIDTH,
									HEIGHT,
									SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	if(mainWindow == NULL) cout << "SDL_CreateWindow: " << SDL_GetError() << endl;
	SDL_GL_CreateContext(mainWindow);
	SDL_GL_SetSwapInterval(1);
}

// Initialize OpenGL
void initOpenGL() {
	double ratio = ((double)WIDTH) / ((double)HEIGHT);

	glShadeModel(GL_SMOOTH);
	glClearColor(1, 1, 1, 1);
	glClearDepth(1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(	-tan( 45.0/360*PI ) * 0.1 * ratio,
				tan( 45.0/360*PI ) * 0.1 * ratio,
				-tan( 45.0/360*PI ) * 0.1,
				tan( 45.0/360*PI ) * 0.1,
				0.1,
				100 );

}

// Load the textures for later use.
void initTextures() {
	textureSurface = new SDL_Surface *[6];
	texture = new GLuint[6];

	for( int i = 0; i < 6; i++) {
		createTexture(i);
	}
	
	delete[] textureSurface;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Each texture being created goes through the same method calls and is named with a number, referenced by index.
void createTexture(int index) {
	textureSurface[index] = SDL_LoadBMP( ("media\\" + to_string( (long long)index ) + ".bmp").c_str() );
	if(textureSurface[index] == NULL) cout << "IMG_Load: " << SDL_GetError() << endl;

	glGenTextures(1, &texture[index]);
	glBindTexture(GL_TEXTURE_2D, texture[index]);
	glTexImage2D(	GL_TEXTURE_2D, 0, 3,
					textureSurface[index]->w, textureSurface[index]->h, 0,
					GL_BGR, GL_UNSIGNED_BYTE, textureSurface[index]->pixels );
	glTexParameteri( GL_TEXTURE_2D,
						GL_TEXTURE_MIN_FILTER,
						GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D,
						GL_TEXTURE_MAG_FILTER,
						GL_LINEAR );

	SDL_FreeSurface( textureSurface[index] );
}

