/*
* Name:			Desiree Trepanier
* Project:		PG2-3 - Rubix Cube
* Description:	Textured rubix cube using OpenGL that can be rotated and whose faces can be rotated like that
*				of an actual rubix cube.
*/
#include "main.h"

SDL_Window *mainWindow;
SDL_Surface **textureSurface;
GLuint *texture;
GLuint underCube;

double rotX, rotY, rotZ, rotVel, negRotVel;
bool shiftIsDown;

Quad quads[6][3][3];
double rotVelocityOf[3] = { 0.0 };
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
	rotX = rotY = rotZ = 0.0;
	rotVel = 0.5;
	negRotVel = rotVel * -1.0;

	// The 54 quads that'll be manipulated as the rubix cube moves.
	// origTexture refers to the quad's original texture when the cube spawns.
	// texCol and texRow refer to the section of the texture to be drawn.
	for( int i = 0; i < 6; i++ ) {
		for( int j = 0; j < 3; j++ ) {
			for( int k = 0; k < 3; k++ ) {
				quads[i][j][k].origTexture = i;
				quads[i][j][k].texCol = j;
				quads[i][j][k].texRow = k;
				quads[i][j][k].angle = 0;
			}
		}
	}
	
	initWindow();
	initOpenGL();
	initTextures();
	initDisplayList();
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

// The black cube under the quads to a display list because it will never change
void initDisplayList() {
	underCube = glGenLists(1);

	glNewList( underCube, GL_COMPILE );
		glColor3f(0, 0, 0);
		glBegin(GL_QUADS);
			for( int i = 0; i < 6; i++ )
				drawCubeFace(i);
		glEnd();
	glEndList();
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
	checkKeysForCubeRotation(event);
	checkKeysForSideRotation(event);

	if(event.type == SDLK_ESCAPE) {
		running = false;
	}
}

// The arrow keys, Z, A, and space all manipulate the cube's rotation in some form.
void checkKeysForCubeRotation(SDL_Event &event) {
	switch(event.key.keysym.sym) {
		case SDLK_LEFT:
			rotVelocityOf[X] = rotVel;
			break;
		case SDLK_RIGHT:
			rotVelocityOf[X] = negRotVel;
			break;
		case SDLK_UP:
			rotVelocityOf[Y] = rotVel;
			break;
		case SDLK_DOWN:
			rotVelocityOf[Y] = negRotVel;
			break;
		case SDLK_a:
			rotVelocityOf[Z] = rotVel;
			break;
		case SDLK_z:
			rotVelocityOf[Z] = negRotVel;
			break;
		case SDLK_SPACE:
			for(int i = 0; i < 3; i++) {
				rotVelocityOf[i] = 0;
			}	
			break;
	}
}

// Keys 1-6 rotate a side off the cube either CW or CCW, based on whether or not shift is held down.
void checkKeysForSideRotation(SDL_Event &event) {
	switch(event.key.keysym.sym) {
		case SDLK_1: // Front (2)
			rotateSide(2, shiftIsDown);
			break;
		case SDLK_2: // Bottom (1)
			rotateSide(1, shiftIsDown);
			break;
		case SDLK_3: // Back (3)
			rotateSide(3, shiftIsDown);
			break;
		case SDLK_4: // Top (0)
			rotateSide(0, shiftIsDown);
			break;
		case SDLK_5: // Right (5)
			rotateSide(5, shiftIsDown);
			break;
		case SDLK_6: // Left (4)
			rotateSide(4, shiftIsDown);
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym) {
				case SDLK_LSHIFT:
				case SDLK_RSHIFT:
					shiftIsDown = true;
					break;
			}
		case SDL_KEYUP:
			switch(event.key.keysym.sym) {
				case SDLK_LSHIFT:
				case SDLK_RSHIFT:
					shiftIsDown = false;
					break;
			}
	}
}

// Set the current rotation of the cube and rotate it further if applicable
void rotateCube() {
	glRotatef( rotX, 1, 0, 0 );
	glRotatef( rotY, 0, 1, 0 );
	glRotatef( rotZ, 0, 0, 1 );
	
	rotX += rotVelocityOf[X];
	rotY += rotVelocityOf[Y];
	rotZ += rotVelocityOf[Z];
}

// Rotate to the given face in order to draw the quads.
// Face values: Top = 0, Bottom = 1, Front = 2, Back = 3, Left = 4, Right = 5
void rotateToFace(int face) {
	glPopAndPushMatrix();

	switch(face) {
	case 0:
		glRotatef( 90, 1, 0, 0 );
		break;
	case 1:
		glRotatef( -90, 1, 0, 0 );
		break;
	case 2:
		glRotatef( 0, 0, 1, 0 );
		break;
	case 3:
		glRotatef( 180, 0, 1, 0 );
		break;
	case 4:
		glRotatef( -90, 0, 1, 0 );
		break;
	case 5:
		glRotatef( 90, 0, 1, 0 );
		break;
	}
}

// Rotate the given face either clockwise or counterclockwise.
// Each cube on the given side will rotate with it, meaning that quads on other faces will be affected.
void rotateSide(int face, bool clockwise) {
	int rotation = clockwise ? -90 : 90;

	for( int i = 0; i < 3; i++ ) {
		for( int j = 0; j < 3; j++ ) {
			quads[face][i][j].angle = (quads[face][i][j].angle + rotation) % 360;
		}
	}

	moveCorners(face, clockwise);
	moveNonCorners(face, clockwise);
}

// TODO: Automate
// Manual way to move corners to their next position on face rotation
void moveCorners(int face, bool clockwise) {
	Quad quadOne = quads[face][0][0];

	quads[face][0][0] = quads[face][0][2];
	quads[face][0][2] = quads[face][2][2];
	quads[face][2][2] = quads[face][2][0];
	quads[face][2][0] = quadOne;
}

// TODO: Automate
// Manul way to move non-corners to their next position on face rotation
void moveNonCorners(int face, bool clockwise) {
	Quad quadTwo = quads[face][1][0];
	
	quads[face][1][0] = quads[face][0][1];
	quads[face][0][1] = quads[face][1][2];
	quads[face][1][2] = quads[face][2][1];
	quads[face][2][1] = quadTwo;

	/* WIP -- Current issues: rotating wrong way; 6 being overwritten (making 2 and 6 overwritten)
	for( int i = 1; i < 8; i += 2 ) {
		int getQuadFromI = ( i == 1 || i == 7 ) ? 4 : 2;
		if( i == 3 || i == 7 ) getQuadFromI = getQuadFromI * -1;
		if( clockwise ) getQuadFromI = getQuadFromI * -1;
		int quadNum = i + getQuadFromI;

		cout << "i is " << i << " and quadNum is " << quadNum << endl;

		if( i == 7 )
			quads[face][getColForNum(i)][getRowForNum(i)] = quadTwo;
		else
			quads[face][getColForNum(i)][getRowForNum(i)] = quads[face][getColForNum(quadNum)][getRowForNum(quadNum)];
	}*/
}

// Find the column given a second number. Used when moving quads.
int getColForNum(int num) {
	return num % 3;
}

// Find the row given a second number. Used when moving quads.
int getRowForNum(int num) {
	return num / 3;
}

// To reset turtle position to the last pushed matrix
void glPopAndPushMatrix() {
	glPopMatrix();
	glPushMatrix();
}

// Draw the rubix cube to the window with its current rotation, textures, and position.
// To have a multi-texture cube and prevent clipping, additional quads are draw slightly
// raised off the main cube.
void drawRubixCube() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	
	glTranslatef (0, 0, -6);
	rotateCube();
	glCallList( underCube );
	drawQuadsOnCube();

	SDL_GL_SwapWindow(mainWindow);
}

// Draw the 54 quads slightly off the black cube to prevent clipping.
// The quads are drawn based on shift the perspective (in two-thirds increments) to the respective
// face (via rotation) and row and column (via translation).
void drawQuadsOnCube() {
	double twoThirds = 2.0/3.0;
	
	glColor3f(1, 1, 1);
	glPushMatrix();

	for( int i = 0; i < 6; i++ ) {
		rotateToFace(i);
		glTranslatef( twoThirds * -1.0, twoThirds * -1.0, 0 ); // Start drawing the quads at the upper-left and front of the face
		drawQuadsOnFace(i, twoThirds);
	}

	glPopMatrix();
}

// Draw the 9 quads on the currently active face via translation and rotate the
// quad according to the quad's current angle.
void drawQuadsOnFace(int face, double increment) {
	for( int j = 0; j < 3; j++ ) {
		glPushMatrix();

		for( int k = 2; k >= 0; k-- ) {
			glPushMatrix();
			glBindTexture( GL_TEXTURE_2D, texture[quads[face][j][k].origTexture] );
			glRotatef( (float)quads[face][j][k].angle, 0, 0, 1 );
			
			glBegin(GL_QUADS);
				drawQuad( quads[face][j][k].texCol, quads[face][j][k].texRow );
			glEnd();

			glPopMatrix();
			glTranslatef( 0, increment, 0 );
		}

		glPopMatrix();

		glTranslatef( increment, 0, 0 );
	}
}

// Draw the face of the rubix cube. Each face has 12 vertices.
// Face values: Top = 0, Bottom = 1, Front = 2, Back = 3, Left = 4, Right = 5
void drawCubeFace(int face) {
	glTexCoord2f( 0, 1 ); glVertex3f( vertices[face][0],  vertices[face][1],  vertices[face][2] );
	glTexCoord2f( 1, 1 ); glVertex3f( vertices[face][3],  vertices[face][4],  vertices[face][5] );
	glTexCoord2f( 1, 0 ); glVertex3f( vertices[face][6],  vertices[face][7],  vertices[face][8] );
	glTexCoord2f( 0, 0 ); glVertex3f( vertices[face][9],  vertices[face][10], vertices[face][11]);
}

// Draw a quad on the currently active face based on the given column and row. 
// Each quad is ~0.33 in size, thus drawn in one-third increments.
void drawQuad(int col, int row) {
	double	oneThird = 1.0/3.0,
			negOneThird = -1.0/3.0,
			zWithOffset = 1.005,
			colX = col/3.0,
			rowY = row/3.0;
	
	glTexCoord2f( colX,				rowY + oneThird );	glVertex3f( negOneThird,	negOneThird,	zWithOffset );
	glTexCoord2f( colX + oneThird,	rowY + oneThird );	glVertex3f( oneThird,		negOneThird,	zWithOffset );
	glTexCoord2f( colX + oneThird,	rowY			);	glVertex3f( oneThird,		oneThird,		zWithOffset );
	glTexCoord2f( colX,				rowY			);	glVertex3f( negOneThird,	oneThird,		zWithOffset );
}