/*
* Name:			Desiree Trepanier
* Project:		PG2-3 - Rubix Cube
* Description:	Textured rubix cube using OpenGL that can be rotated and whose faces can be rotated like that
*				of an actual rubix cube.
*/
#include "main.h"

const int	X = 0,
			Y = 1,
			Z = 2;

SDL_Window *mainWindow;
SDL_Surface **textureSurface;
GLuint *texture;

AxisRotation movingRotation[3];
Quad quads[6][3][3];

double rotX, rotY, rotZ, rotAmt;
bool shift;

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
	for( int i = 0; i < 3; i++ ) {
		movingRotation[i].posMovement = false;
		movingRotation[i].negMovement = false;
	} 

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
		case SDLK_1: // Front (2)
			rotateFace(2, true);
			break;
		case SDLK_2: // Bottom (1)
			rotateFace(1, true);
			break;
		case SDLK_3: // Back (3)
			rotateFace(3, true);
			break;
		case SDLK_4: // Top (0)
			rotateFace(0, true);
			break;
		case SDLK_5: // Right (5)
			rotateFace(5, true);
			break;
		case SDLK_6: // Left (4)
			rotateFace(4, true);
			break;
		case SDL_KEYDOWN:
			// on keydown + lshift or rshift, shift = true
			// on keyup + lshift or rshift, shift = false
		case SDLK_ESCAPE:
			running = false;
	}
}

// Used to rotate the cube around the proper axis.
void setRotation(int axis, bool posMovement, bool negMovement) {
	movingRotation[axis].posMovement = posMovement;
	movingRotation[axis].negMovement = negMovement;
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
// Each cube on the face will rotate with it, meaning that quads on other faces will be affected.
void rotateFace(int face, bool clockwise) {
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
	
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
		for( int i = 0; i < 6; i++ )
			drawCubeFace(i);
	glEnd();
	
	glColor3f(1, 1, 1);
	drawQuadsOnCube();

	SDL_GL_SwapWindow(mainWindow);
}

// Draw the 54 quads slightly off the black cube to prevent clipping.
// The quads are drawn based on shifting the perspective (in two-thirds increments) to the respective
// face (via rotation) and row and column (via translation).
void drawQuadsOnCube() {
	double twoThirds = 2.0/3.0;
	
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