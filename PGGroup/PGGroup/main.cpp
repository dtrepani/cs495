/*
* Name:			
* Project:		PGGroup
* Description:	
*/
#include "main.h"

SDL_Window *mainWindow;

int main(int argc, char **argv) {
	init();
	pollEventsAndDraw();

	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
	return 0;
}

// Initialize variables and call initalizations for the window and openGL
void init() {
	initWindow();
	initOpenGL();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Initalize window
void initWindow() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	mainWindow = SDL_CreateWindow(	"Game",
									SDL_WINDOWPOS_UNDEFINED,
									SDL_WINDOWPOS_UNDEFINED, 
									WIDTH, HEIGHT, 
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
				0.1, 100 );
}

// Better suited in the level superclass
// texture may be used when the same texture is needed multiple times throughout a level, such as for the aisles in the tutorial.
// In this case, create the texture beforehand and pass it in.
Entity* createEntity(string name, GLuint* texture, GLfloat* vertices, float x, float y, float z) { 
	return new Entity(new Vector(x, y, z), (texture == NULL) ? createTexture(name) : texture, vertices);
}
PlaneEntity* createPlaneEntity(string name, GLuint* texture, Orientation orientation, GLfloat* vertices, float x, float y, float z) { 
	return new PlaneEntity(new Vector(x, y, z), (texture == NULL) ? createTexture(name) : texture, vertices, orientation);
}
InteractableEntity* createInteractableEntity(string name, GLuint* texture, GLfloat* vertices, float x, float y, float z) { 
	return new InteractableEntity(new Vector(x, y, z), (texture == NULL) ? createTexture(name) : texture, vertices);
}
PlayerEntity* createPlayerEntity(float x, float y, float z) { 
	return new PlayerEntity(new Vector(x, y, z), NULL, NULL); 
}
WizardEntity* createWizardEntity(string name, GLfloat* vertices, float x, float y, float z) { 
	return new WizardEntity(new Vector(x, y, z), createTexture(name), vertices); 
}

// Each texture being created goes through the same method calls and is named with a number, referenced by index.
GLuint* createTexture(string name) {
	GLuint* texture = new GLuint;
	SDL_Surface* textureSurface = SDL_LoadBMP( ("resources\\" + name + ".bmp").c_str() );
	if(textureSurface == NULL) cout << "SDL_LoadBMP: " << SDL_GetError() << endl;

	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexImage2D(	GL_TEXTURE_2D, 0, 3,
					textureSurface->w, textureSurface->h, 0,
					GL_BGR, GL_UNSIGNED_BYTE, textureSurface->pixels );
	glTexParameteri( GL_TEXTURE_2D,
						GL_TEXTURE_MIN_FILTER,
						GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D,
						GL_TEXTURE_MAG_FILTER,
						GL_LINEAR );

	SDL_FreeSurface( textureSurface );

	return texture;
}

void pollEventsAndDraw() {
	SDL_Event event;
	bool running = true;
	bool keys[282] = { false };
	
	// ========== START TEST ========== //
#ifdef cubeVertNotes
	GLfloat cube[72] = { // Front face
-1.0, -1.0,  1.0,
 1.0, -1.0,  1.0,
 1.0,  1.0,  1.0,
-1.0,  1.0,  1.0,
// Back face
-1.0, -1.0, -1.0,
-1.0,  1.0, -1.0,
 1.0,  1.0, -1.0,
 1.0, -1.0, -1.0,
// Top face
-1.0,  1.0, -1.0,
-1.0,  1.0,  1.0,
 1.0,  1.0,  1.0,
 1.0,  1.0, -1.0,
// Bottom face
-1.0, -1.0, -1.0,
 1.0, -1.0, -1.0,
 1.0, -1.0,  1.0,
-1.0, -1.0,  1.0,
// Right face
 1.0, -1.0, -1.0,
 1.0,  1.0, -1.0,
 1.0,  1.0,  1.0,
 1.0, -1.0,  1.0,
// Left face
-1.0, -1.0, -1.0,
-1.0, -1.0,  1.0,
-1.0,  1.0,  1.0,
-1.0,  1.0, -1.0}; // default cube
#endif
	bool collision[5] = {false};
	GLfloat modelVert[12] = {
		-1.0, -1.0,  0,
		 1.0, -1.0,  0,
		 1.0,  1.0,  0,
		-1.0,  1.0,  0};
	Entity* tmpModel = createEntity("1", NULL, modelVert, 0, 1.0f, -10.0f);
	tmpModel->addCollider(0, 0, 0, 0);
	tmpModel->incrementYOf(ROTATION, 20.0f);

	GLfloat floorVert[12] = { 
		-3.0, 0.0,  40.0,
		 3.0, 0.0,  40.0,
		 3.0, 0.0, -40.0,
		-3.0, 0.0, -40.0};
	PlaneEntity* tmpFloor = createPlaneEntity("2", NULL, HORIZONTAL, floorVert, 0, 0, 0);

	GLuint* wallTex = createTexture("3");
	GLfloat wallVert[12] = { 
		0.0, 0.0,  30.0,
		0.0, 10.0,  30.0,
		0.0, 10.0, -40.0,
		0.0, 0.0, -40.0};
	PlaneEntity* tmpWall1 = createPlaneEntity("", wallTex, VERTICAL_Z, wallVert, -3.0f, 0, 0);
	PlaneEntity* tmpWall2 = createPlaneEntity("", wallTex, VERTICAL_Z, wallVert, 3.0f, 0, 0);

	PlayerEntity* player = createPlayerEntity(0, 1.0f, 0);
	// ========== END TEST ========== //

	while( running ) {
		if( SDL_PollEvent(&event) ) {
			if(event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_QUIT) {
				running = false;
			} else if(event.type == SDL_KEYDOWN) {
				if(event.key.keysym.sym == SDLK_SPACE) player->jump();
				else if(event.key.keysym.sym == SDLK_x) player->incrementYOf(ROTATION, 180.0f);
				keys[event.key.keysym.scancode] = true;
			} else if(event.type == SDL_KEYUP) {
				keys[event.key.keysym.scancode] = false;
			}
		}
		movePlayer(keys, player);

		// ========== START TEST ========== //

		player->incrementYOf(VELOCITY, -0.2f); // gravity hack

			// Must be  in level superclass: ******
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glLoadIdentity();

		glColor3f(1, 1, 1);

		GLfloat matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

		// Check collisions before drawing player
		// TO-DO: Remove these! Temporary hacks to test stuff
		if(player->hasCollided(tmpModel)) {
			if(!collision[0]) {
				cout << "Collision: Model" << endl;
				collision[0] = true;
			}
		} else if(collision[0] == true) collision[0] = false;
		
		if(player->hasCollided(tmpWall1)) {
			if(!collision[1]) {
				cout << "Collision: Wall1" << endl;
				collision[1] = true;
			}
		} else if(collision[1] == true) collision[1] = false;
		
		if(player->hasCollided(tmpWall2)) {
			if(!collision[2]) {
				cout << "Collision: Wall2" << endl;
				collision[2] = true;
			}
		} else if(collision[2] == true) collision[2] = false;

		if(player->hasCollided(tmpFloor)) {
			if(!collision[3]) {
				cout << "Collision: Floor" << endl;
				collision[3] = true;
			}
		} else if(collision[3] == true) collision[3] = false;
		
		player->checkForCollision(tmpModel);
		player->checkForCollision(tmpFloor);
		player->checkForCollision(tmpWall1);
		player->checkForCollision(tmpWall2);

		player->drawSelf(matrix); // Used to adjust camera based on player position
		glLoadMatrixf(matrix);
			// ******************************* //

		//tmpModel->incrementXOf(ROTATION, 5.0f);
		//tmpModel->incrementZOf(POSITION, 0.05f);
		
		
		tmpFloor->drawSelf();
		tmpWall1->drawSelf();
		tmpWall2->drawSelf();
		tmpModel->drawSelf();

		// ========== END TEST ========== //
		
		SDL_GL_SwapWindow(mainWindow);
	}
	
		// ========== START TEST ========== //
	// Remember to delete all models (should be entity linked list) when done!
	delete tmpModel;
	delete tmpWall1;
	delete tmpWall2;
	delete tmpFloor;
	delete player;
		// ========== END TEST ========== //
}

void movePlayer(bool* keys, PlayerEntity* player) {
		if(keys[SDL_GetScancodeFromKey(SDLK_UP)] || keys[SDL_GetScancodeFromKey(SDLK_w)]) player->moveForward(true);
		if(keys[SDL_GetScancodeFromKey(SDLK_DOWN)] || keys[SDL_GetScancodeFromKey(SDLK_s)]) player->moveForward(false);
		if(keys[SDL_GetScancodeFromKey(SDLK_q)] ) player->strafe(true);
		if(keys[SDL_GetScancodeFromKey(SDLK_e)] ) player->strafe(false);
		if(keys[SDL_GetScancodeFromKey(SDLK_LEFT)] || keys[SDL_GetScancodeFromKey(SDLK_a)]) player->incrementYOf(ROTATION, 1.5f);
		if(keys[SDL_GetScancodeFromKey(SDLK_RIGHT)] || keys[SDL_GetScancodeFromKey(SDLK_d)]) player->incrementYOf(ROTATION, -1.5f);
}
