/*
* Name:			Desiree Trepanier
* Project:		PG2-3 - Rubix Cube
* Description:	Textured rubix cube using OpenGL that can be rotated.
*/
#include "main.h"

const int	X = 0,
			Y = 1,
			Z = 2;

SDL_Window *mainWindow;
SDL_Surface **textureSurface;
GLuint *texture;

AxisRotation movingRotation[3];

double rotX, rotY, rotZ, rotAmt;

float vertices[6][12] = {
	// Top face
  { -1,  1,  1,
	 1,  1,  1,
	 1,  1, -1,
	-1,  1, -1  },

	// Bottom face
  { -1, -1, -1,
	 1, -1, -1,
	 1, -1,  1,
	-1, -1,  1  },

	// Front face
  { -1, -1,  1,
	 1, -1,  1,
	 1,  1,  1,
	-1,  1,  1  },
	
	// Back face
  {  1, -1, -1,
	-1, -1, -1,
	-1,  1, -1,
	 1,  1, -1  },

	// Left face
  { -1, -1, -1,
	-1, -1,  1,
	-1,  1,  1,
	-1,  1, -1  },

	// Right face
  {  1, -1,  1,
	 1, -1, -1,
	 1,  1, -1,
	 1,  1,  1  },
};

int main(int argc, char **argv) {
	init();
	pollEventsAndDraw();

	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
	return 0;
}

// Initialize variables and call initalizations for the window, openGL, and textures
void init() {
	rotX = rotY = rotZ = 0;
	rotAmt = 0.5;

	// Each axis will have no rotation when first initalized.
	AxisRotation tmp = {false, false};
	for( int i = 0; i < 3; i++ ) {
		movingRotation[i] = tmp;
	}
	
	initWindow();
	initOpenGL();
	initTextures();
}

// Initalize window
void initWindow() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	mainWindow = SDL_CreateWindow(	"Rubix Cube",
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


// Load the textures for later use when drawing the faces of the rubix cube.
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

// Checks for keyboard and button events, in which it will react to accordingly, then draws the rubix cube.
// Clicking the window close button or ESC key breaks out of the loop in order to unitialize objects before
// the program exits.
void pollEventsAndDraw() {
	SDL_Event event;
	bool running = true;

	while( running ) {
		if( SDL_PollEvent(&event) )
			if( event.type == SDL_QUIT )
				running = false;
			else if( event.type == SDL_KEYDOWN )
				checkKeys(event, running);

		drawRubixCube();
	}
}

// If any of the arrow keys, Z, or A have been pressed, rotate the cube accordingly.
// If space has been pressed, stop all rotation. ESC will close the program.
void checkKeys(SDL_Event &event, bool &running) {
	switch(event.key.keysym.sym) {
		case SDLK_LEFT:
			setRotation(X, true, false);
			break;
		case SDLK_RIGHT:
			setRotation(X, false, true);
			break;
		case SDLK_UP:
			setRotation(Y, true, false);
			break;
		case SDLK_DOWN:
			setRotation(Y, false, true);
			break;
		case SDLK_a:
			setRotation(Z, true, false);
			break;
		case SDLK_z:
			setRotation(Z, false, true);
			break;
		case SDLK_SPACE:
			for(int i = 0; i < 3; i++) {
				movingRotation[i].posMovement = false;
				movingRotation[i].negMovement = false;
			}	
			break;
		case SDLK_ESCAPE:
			running = false;
	}
}

// Used to rotate the cube around the proper axis.
void setRotation(int axis, bool posMovement, bool negMovement) {
	movingRotation[axis].posMovement = posMovement;
	movingRotation[axis].negMovement = negMovement;
}

// Draw the rubix cube to the window with its current rotation, textures, and position.
// To have a multi-texture cube and prevent clipping, additional quads are draw slightly
// raised off the main cube.
void drawRubixCube() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	
	glTranslatef (0, 0, -6);
	glColor3f(0, 0, 0);

	rotateCube();
	
	glBegin(GL_QUADS);
		for( int i = 0; i < 6; i++ )
			drawQuad(i, 0);
	glEnd();
	
	glColor3f(1, 1, 1);

	for( int i = 0; i < 6; i++ ) {
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glBegin(GL_QUADS);
			drawQuad(i, 0.005);
		glEnd();
	}

	SDL_GL_SwapWindow(mainWindow);
}

// Set the current rotation of the cube and rotate it further if applicable
void rotateCube() {
	glRotatef( rotX, 1, 0, 0 );
	glRotatef( rotY, 0, 1, 0 );
	glRotatef( rotZ, 0, 0, 1 );

	rotX += getRotationAmount(X);
	rotY += getRotationAmount(Y);
	rotZ += getRotationAmount(Z);
}

// Used to determine if an axis is moving at all. This is determined by whether posMovement
// or negMovement is true. If it is moving in the positive direction, return the positive
// rotation amount. If negative direction, return negative rotation amount. Else, it's not moving.
double getRotationAmount(int axis) {
	if( movingRotation[axis].posMovement ) return rotAmt;
	else if( movingRotation[axis].negMovement ) return -rotAmt;
	else return 0;
}

// Draw the quads to the face of a cube. Each face has 12 vertices.
// Face values: Top = 0, Bottom = 1, Front = 2, Back = 3, Left = 4, Right = 5
void drawQuad(int face, double offset) {
	glTexCoord2f( 0, 1 ); glVertex3f( getVertex( face, 0, offset ),  getVertex( face, 1, offset ),  getVertex( face, 2, offset) );
	glTexCoord2f( 1, 1 ); glVertex3f( getVertex( face, 3, offset ),  getVertex( face, 4, offset ),  getVertex( face, 5, offset) );
	glTexCoord2f( 1, 0 ); glVertex3f( getVertex( face, 6, offset ),  getVertex( face, 7, offset ),  getVertex( face, 8, offset) );
	glTexCoord2f( 0, 0 ); glVertex3f( getVertex( face, 9, offset ), getVertex( face, 10, offset ), getVertex( face, 11, offset) );
}

// Returns the adjusted float value of the vertex based on the offset.
float getVertex(int faceIndex, int vertexIndex, double offset) {
	if( vertices[faceIndex][vertexIndex] < 0 ) offset = -1 * offset;
	return vertices[faceIndex][vertexIndex] + offset;
}