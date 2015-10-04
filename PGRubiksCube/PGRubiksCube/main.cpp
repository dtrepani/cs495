/*
* Name:			Desiree Trepanier
* Project:		PG2-3 - Rubiks Cube
* Description:	Textured rubik's cube using OpenGL that can be rotated and whose faces can be rotated like that
*				of an actual rubik's cube.
*/
#include "main.h"

SDL_Window *mainWindow;
SDL_Surface **textureSurface;
GLuint *texture;
GLuint underCube;

double rotX, rotY, rotZ, rotVel, negRotVel;
bool clockwise;

Quad quads[6][3][3];
Face faces[6];
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

	// Left face
  { -1, -1, -1,
	-1, -1,  1,
	-1,  1,  1,
	-1,  1, -1  },

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
	clockwise = true;
	rotX = rotY = rotZ = 0.0;
	rotVel = 0.5;
	negRotVel = rotVel * -1.0;

	// The 54 quads that'll be manipulated as the rubik's cube moves.
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

	initFaces();
	initWindow();
	initOpenGL();
	initTextures();
	initDisplayList();
}

// The faces need to know what faces are adjacent to them when rotating sides.
void initFaces() {
	for( int i = 2; i < 6; i++ ) {
		faces[i].top = 0;
		faces[i].bottom = 1;

		faces[i].right = ( i == 5 ) ? 2 : i + 1;
		faces[i].left = ( i == 2 ) ? 5 : i - 1;
	}
	
	faces[0].top = faces[1].bottom = 3;
	faces[1].top = faces[0].bottom = 5;
	faces[0].left = faces[1].right = 2;
	faces[1].left = faces[0].right = 4;
}

// Initalize window
void initWindow() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	mainWindow = SDL_CreateWindow(	"Rubiks Cube",
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

// Load the textures for later use when drawing the faces of the rubik's cube.
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

// Checks for keyboard and button events, in which it will react to accordingly, then draws the rubik's cube.
// Clicking the window close button or ESC key breaks out of the loop in order to unitialize objects before
// the program exits.
void pollEventsAndDraw() {
	SDL_Event event;
	bool running = true;

	while( running ) {
		if( SDL_PollEvent(&event) ) {
			checkKeys(event, running);
		}

		drawRubiksCube();
	}
}

// If any of the arrow keys, Z, or A have been pressed, rotate the cube accordingly.
// If space has been pressed, stop all rotation. ESC will close the program.
void checkKeys(SDL_Event &event, bool &running) {
	checkKeysForCubeRotation(event);
	checkKeysForSideRotation(event);

	if(event.type == SDLK_ESCAPE || event.type == SDL_QUIT) {
		running = false;
	}
}

// The arrow keys, Z, A, and space all manipulate the cube's rotation in some form.
void checkKeysForCubeRotation(SDL_Event &event) {
	if( event.type == SDL_KEYDOWN) {
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
}

// Keys 1-6 rotate a side off the cube either CW or CCW, based on whether or not shift is held down.
void checkKeysForSideRotation(SDL_Event &event) {
	if( event.type == SDL_KEYDOWN ) {
			switch(event.key.keysym.sym) {
				case SDLK_1: // Front (3)
					rotateSide(3);
					break;
				case SDLK_2: // Bottom (1)
					rotateSide(1);
					break;
				case SDLK_3: // Back (5)
					rotateSide(5);
					break;
				case SDLK_4: // Top (0)
					rotateSide(0);
					break;
				case SDLK_5: // Right (4)
					rotateSide(4);
					break;
				case SDLK_6: // Left (2)
					rotateSide(2);
					break;
				case SDLK_LSHIFT:
				case SDLK_RSHIFT:
					clockwise = false;
					break;
			}
	} else if( event.type == SDL_KEYUP ) {
			switch(event.key.keysym.sym) {
				case SDLK_LSHIFT:
				case SDLK_RSHIFT:
					clockwise = true;
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
void rotateToFace(int face) {
	glPopAndPushMatrix();

	switch(face) {
	case 0: // Top
		glRotatef( 90, 1, 0, 0 );
		break;
	case 1: // Bottom
		glRotatef( -90, 1, 0, 0 );
		break;
	case 2: // Left
		glRotatef( -90, 0, 1, 0 );
		break;
	case 3: // Front
		glRotatef( 0, 0, 1, 0 );
		break;
	case 4: // Back
		glRotatef( 90, 0, 1, 0 );
		break;
	case 5: // Right
		glRotatef( 180, 0, 1, 0 );
		break;
	}
}

void swap(Quad &quadOne, Quad &quadTwo) {
	Quad tmp = quadOne;

	quadOne = quadTwo;
	quadTwo = quadOne;
}

// Rotate the given face either clockwise or counterclockwise.
// Each cube on the given side will rotate with it, meaning that quads on other faces will be affected.
void rotateSide(int face) {
	int rotation = clockwise ? -90 : 90;
	Quad quadFaceBefore[3][3];

	for( int i = 0; i < 3; i++ ) {
		for( int j = 0; j < 3; j++ ) {
			quads[face][i][j].angle = (quads[face][i][j].angle + rotation) % 360;
		}
	}

	getQuadsForFace(face, quadFaceBefore);

	for( int i = 0; i < 3; i++ ) {
		for( int j = 0; j < 3; j++ ) {
			if( !(i == 1 && j == 1) ) { // is not section 5
				if( clockwise ) {
					quads[face][2-j][i] = quadFaceBefore[i][j];
				} else {
					quads[face][j][2-i] = quadFaceBefore[i][j];				
				}
			}
		}
	}

	Quad faceTop[3][3];
	getQuadsForFace(faces[face].top, faceTop);


	/*rotateFaceXToFaceY(face, "LEFT", "TOP");
	rotateFaceXToFaceY(face, "BOTTOM", "LEFT");
	rotateFaceXToFaceY(face, "RIGHT", "BOTTOM");*/
}

void rotateFaceXToFaceY(int srcFace, string faceX, string faceY) {
	int xCol, xRow, xFace, yFace;
	bool xColConstant;

	xFace = getAdjFaceFromString(srcFace, faceX);
	yFace = getAdjFaceFromString(srcFace, faceY);

	findWhichQuadsToRotate(faceX, xCol, xRow, xColConstant);

	for(int i = 0; i < 3; i++) {
		int col, row;
		if(xColConstant) {
			col = xCol;
			row = i;
		} else {
			col = i;
			row = xRow;
		}

		if( clockwise ) {
			quads[yFace][2-row][col] = quads[xFace][col][row];
		} else {
			quads[yFace][row][2-col] = quads[xFace][col][row];				
		}
	}
}

void findWhichQuadsToRotate(string adjFace, int &col, int &row, bool &colConstant) {
	if( adjFace == "TOP" ) {
		col = 0;
		row = 2;
		colConstant = false;
	} else if( adjFace == "BOTTOM" ) {
		col = 0;
		row = 0;
		colConstant = false;
	} else if( adjFace == "LEFT" ) {
		col = 2;
		row = 0;
		colConstant = true;
	} else if( adjFace == "RIGHT" )	{
		col = 0;
		row = 0;
		colConstant = true;
	}
}

int getAdjFaceFromString(int face, string adjFace) {
	if( adjFace == "TOP" )		return faces[face].top;
	if( adjFace == "BOTTOM" )	return faces[face].bottom;
	if( adjFace == "LEFT" )		return faces[face].left;
	if( adjFace == "RIGHT" )	return faces[face].right;

	return false;
}

// Copy the quads for a specific face into a 2d array.
// Used to 
void getQuadsForFace(int face, Quad (&quadFace)[3][3]) {
	memcpy(&quadFace[0][0], &quads[face][0][0], sizeof(quadFace[0][0]) * 9);
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

// Draw the rubik's cube to the window with its current rotation, textures, and position.
// To have a multi-texture cube and prevent clipping, additional quads are draw slightly
// raised off the main cube.
void drawRubiksCube() {
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

// Draw the face of the rubik's cube. Each face has 12 vertices.
// Face values: Top = 0, Bottom = 1, Left = 2, Front = 3, Right = 4, Back = 5
void drawCubeFace(int face) {
	glTexCoord2f( 0, 1 ); glVertex3f( vertices[face][0],  vertices[face][1],  vertices[face][2] );
	glTexCoord2f( 1, 1 ); glVertex3f( vertices[face][3],  vertices[face][4],  vertices[face][5] );
	glTexCoord2f( 1, 0 ); glVertex3f( vertices[face][6],  vertices[face][7],  vertices[face][8] );
	glTexCoord2f( 0, 0 ); glVertex3f( vertices[face][9],  vertices[face][10], vertices[face][11]);
}

// Draw a quad on the currently active face based on the given column and row. 
// Each quad is ~0.33 in size, thus drawn in one-third increments.
void drawQuad(int col, int row) {
	double	oneThird = 0.325,
			negOneThird = -0.325,
			zWithOffset = 1.005,
			colX = col/3.0,
			rowY = row/3.0;
	
	glTexCoord2f( colX,				rowY + oneThird );	glVertex3f( negOneThird,	negOneThird,	zWithOffset );
	glTexCoord2f( colX + oneThird,	rowY + oneThird );	glVertex3f( oneThird,		negOneThird,	zWithOffset );
	glTexCoord2f( colX + oneThird,	rowY			);	glVertex3f( oneThird,		oneThird,		zWithOffset );
	glTexCoord2f( colX,				rowY			);	glVertex3f( negOneThird,	oneThird,		zWithOffset );
}