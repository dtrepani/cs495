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

double rotX, rotY, rotZ, rotVel, negRotVel;
bool clockwise;

//Quad quads[6][3][3];
Face *face[6];
double rotVelocityOf[3] = { 0.0 };

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
	/*for( int i = 0; i < 6; i++ ) {
		for( int j = 0; j < 3; j++ ) {
			for( int k = 0; k < 3; k++ ) {
				quads[i][j][k].origTexture = i;
				quads[i][j][k].texCol = j;
				quads[i][j][k].texRow = k;
				quads[i][j][k].angle = 0;
			}
		}
	}*/

	initFaces();
	initWindow();
	initOpenGL();
	initTextures();
}

// The faces need to know what faces are adjacent to them when rotating sides.
void initFaces() {
	for( int i = 0; i < 6; i++ ) {
		face[i] = new Face(i);
	}

	for( int i = 2; i < 6; i++ ) {
		face[i]->setAdjFaces( face[0], face[1], face[( i == 2 ) ? 5 : i - 1], face[( i == 5 ) ? 2 : i + 1] );
	}
	
	face[0]->setAdjFaces( face[3], face[5], face[4], face[2] );
	face[1]->setAdjFaces( face[5], face[3], face[4], face[2] );
}

// Initalize window
void initWindow() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	mainWindow = SDL_CreateWindow(	"Rubik's Cube",
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
					face[3]->rotateAbout(clockwise);
					break;
				case SDLK_2: // Bottom (1)
					face[1]->rotateAbout(clockwise);
					break;
				case SDLK_3: // Back (5)
					face[5]->rotateAbout(clockwise);
					break;
				case SDLK_4: // Top (0)
					face[0]->rotateAbout(clockwise);
					break;
				case SDLK_5: // Right (4)
					face[4]->rotateAbout(clockwise);
					break;
				case SDLK_6: // Left (2)
					face[2]->rotateAbout(clockwise);
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

// The face knows where it needs to rotate to.
// Pass the current perspective to the face and load the returned perspective rotation.
void rotateToFace(int faceNum) {
	GLfloat matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	face[faceNum]->rotateToSelf(matrix);
	glLoadMatrixf(matrix);
}

// Draw the rubik's cube to the window with its current rotation, textures, and position.
// To have a multi-texture cube and prevent clipping, additional quads are draw slightly
// raised off the main cube.
void drawRubiksCube() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	glTranslatef (0, 0, -6);
	
	rotateCube();
	
	glColor3f(0, 0, 0);
	glPushMatrix();
	for( int i = 0; i < 6; i++ ) {
		drawFace(i);
	}
	glPopMatrix();

	SDL_GL_SwapWindow(mainWindow);
}

// The face knows where to draw itself.
// Pass the current perspective to the face for it to use to draw itself then reset the perspective from
// the rotation done in rotateToFace.
void drawFace(int faceNum) {
	rotateToFace(faceNum);

	glBegin(GL_QUADS);
		GLfloat matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		face[faceNum]->drawSelf(texture, matrix);
	glEnd();

	glPopMatrix();
	glPushMatrix();
}

#ifdef _TMP_
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
	
	/* TC
	for(int i = 0; i < 6; i++) {
		printf("Face %d: (%d, %d, %d, %d)", i, faces[i].top, faces[i].bottom, faces[i].left, faces[i].right);
	}*/

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

	/*Quad faceTop[3][3];
	getQuadsForFace(faces[face].top, faceTop);
	for( int i = 0; i < 3; i++ ) { // CW: Top = Left; CCW: Top = Right
		int col;
		if( clockwise ) {
			col = 2;
			quads[faces[face].top][i][2-col] = quads[faces[face].left][col][i];
		} else {
			col = 0;
			quads[faces[face].top][2-i][col] = quads[faces[face].right][col][i];
		}
	}
	
	for( int i = 0; i < 3; i++ ) { // CW: Left = Bottom; CCW: Right = Bottom
		int row;
		if( clockwise ) {
			row = 2;
			quads[faces[face].left][row][2-i] = quads[faces[face].bottom][i][row];
		} else {
			row = 0;
			quads[faces[face].right][2-row][i] = quads[faces[face].bottom][i][row];
		}
	}

	for( int i = 0; i < 3; i++ ) { // CW: Bottom = Right; CCW: Bottom = Left
		int col;
		if( clockwise ) {
			col = 0;
			quads[faces[face].bottom][i][2-col] = quads[faces[face].right][col][i];
		} else {
			col = 2;
			quads[faces[face].bottom][2-i][col] = quads[faces[face].left][col][i];
		}
	}
	
	for( int i = 0; i < 3; i++ ) { // CW: Right = Top (Saved); CCW: Left = Top (Saved)
		int row;
		if( clockwise ) {
			row = 0;
			quads[faces[face].right][row][2-i] = faceTop[i][row];
		} else {
			row = 2;
			quads[faces[face].left][2-row][i] = faceTop[i][row];			
		}
	}*/

	/*rotateFaceXToFaceY(face, "LEFT", "TOP");
	rotateFaceXToFaceY(face, "BOTTOM", "LEFT");
	rotateFaceXToFaceY(face, "RIGHT", "BOTTOM");*/
}

#endif
