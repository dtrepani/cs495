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
	//glClearColor(1, 1, 1, 1);
	glClearColor(0.35f, 0.6f, 1, 1);
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

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

// Better suited in the level superclass
// texture may be used when the same texture is needed multiple times throughout a level, such as for the aisles in the tutorial.
// In this case, create the texture beforehand and pass it in.
Entity* createEntity(string name, GLuint* texture, GLfloat* vertices, float radius, float x, float y, float z) { 
	return new Entity(new Vector(x, y, z), (texture == NULL) ? createTexture(name) : texture, vertices, radius);
}
PlaneEntity* createPlaneEntity(string name, GLuint* texture, Orientation orientation, GLfloat* vertices, float x, float y, float z) {
	return new PlaneEntity(new Vector(x, y, z), (texture == NULL) ? createTexture(name) : texture, vertices, orientation);
}
BlockEntity* createBlockEntity(string name, GLuint* texture, float x, float y, float z, float widthX, float heightY, float lengthZ) {
	return new BlockEntity(new Vector(x, y, z), (texture == NULL) ? createTexture(name) : texture, widthX, heightY, lengthZ);
}
InteractableEntity* createInteractableEntity(string name, GLuint* texture, GLfloat* vertices, float radius, float x, float y, float z) { 
	return new InteractableEntity(new Vector(x, y, z), (texture == NULL) ? createTexture(name) : texture, vertices, radius);
}
PlayerEntity* createPlayerEntity(float x, float y, float z, float radius) { 
	return new PlayerEntity(new Vector(x, y, z), radius); 
}
WizardEntity* createWizardEntity(string name, GLfloat* vertices, float radius, float x, float y, float z) { 
	return new WizardEntity(new Vector(x, y, z), createTexture(name), vertices, radius); 
}

// Each texture being created goes through the same method calls and is in png format.
GLuint* createTexture(string name) {
	int mode;
	GLuint* texture = new GLuint;
	SDL_Surface* textureSurface = IMG_Load( ("resources\\" + name + ".png").c_str() );
	if(textureSurface == NULL) cout << "IMG_Load: " << SDL_GetError() << endl;

	switch (textureSurface->format->BytesPerPixel) {
        case 4:
            if (textureSurface->format->Rmask == 0x000000ff)
                mode = GL_RGBA;
            else
                mode = GL_BGRA;
            break;
        case 3:
            if (textureSurface->format->Rmask == 0x000000ff)
                mode = GL_RGB;
            else
                mode = GL_BGR;
            break;
	}

	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexImage2D( GL_TEXTURE_2D, 0, textureSurface->format->BytesPerPixel, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels );
	glTexParameteri( GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	SDL_FreeSurface( textureSurface );
	return texture;
}

void pollEventsAndDraw() {
    static GLint initialTime = 0;
    static GLint frames = 0;
	SDL_Event event;
	bool running = true;
	bool keys[282] = { false };
	
	// ========== START TEST ========== //
	bool collision[5] = {false};
	LinkedList* entities = new LinkedList();

	// NPCs
	PlayerEntity* player = createPlayerEntity(0, 1.0f, 0, NULL);
	//SkyboxBlockEntity* skybox = new SkyboxBlockEntity();
	GLfloat modelVert[12] = {
		-0.5f, -850.0f/400.0f*0.5f,  0,
		 0.5f, -850.0f/400.0f*0.5f,  0,
		 0.5f,  850.0f/400.0f*0.5f,  0,
		-0.5f,  850.0f/400.0f*0.5f,  0};
	WizardEntity* wizard = createWizardEntity("wizard", &modelVert[0], NULL, 0, 1.5f, -7.0f);

	// FLOOR
	GLfloat floorVert[12] = { 
		-6.0, 0.0,  40.0,
		 6.0, 0.0,  40.0,
		 6.0, 0.0, -40.0,
		-6.0, 0.0, -40.0};
	PlaneEntity* tmpFloor = createPlaneEntity("spaceship_floor", NULL, HORIZONTAL, &floorVert[0], 0, 0, 0);

	// WALLS
	GLuint* wallTex = createTexture("spaceship_wall");
	GLfloat wallVert[12] = { 
		0.0, 0.0,  30.0,
		0.0, 10.0,  30.0,
		0.0, 10.0, -40.0,
		0.0, 0.0, -40.0};
	PlaneEntity* tmpWall1 = createPlaneEntity("", wallTex, VERTICAL_Z, &wallVert[0], -6.0f, 0, 0);
	PlaneEntity* tmpWall2 = createPlaneEntity("", wallTex, VERTICAL_Z, &wallVert[0], 6.0f, 0, 0);
	
	// INTERACTABLES
	GLfloat coinVert[12] = { 
		-0.25f, -25.0f/75.0f*0.25f,  0.0,
		 0.25f, -25.0f/75.0f*0.25f,  0.0,
		 0.25f, 25.0f/75.0f*0.25f, 0.0,
		-0.25f, 25.0f/75.0f*0.25f, 0.0};
	CoinInteractableEntity* tmpCoin = new CoinInteractableEntity(new Vector(0, 0.1f, -5.0f), createTexture("coin"), coinVert, 2.0f);
	
	GLfloat shipVert[12] = { 
		-2.0f, -800.0f/360.0f*2.0f,  0.0,
		 2.0f, -800.0f/360.0f*2.0f,  0.0,
		 2.0f, 800.0f/360.0f*2.0f, 0.0,
		-2.0f, 800.0f/360.0f*2.0f, 0.0};
	InteractableEntity* tmpShip = new InteractableEntity(new Vector(0, 4.2f, -15.0f), createTexture("spaceship"), shipVert, 2.0f);

	GLfloat gumballMachineVert[12] = { 
		-0.3f, -2.4f*0.3f,  0.0,
		 0.3f, -2.4f*0.3f,  0.0,
		 0.3f, 2.4f*0.3f, 0.0,
		-0.3f, 2.4f*0.3f, 0.0};
	InteractableEntity* tmpGumballMachine = new InteractableEntity(new Vector(3.0f, 0.65f, -10.0f), createTexture("gumballmachine"), gumballMachineVert, NULL);

	// MISC
		// "Step" with planes
	GLuint* stepTex = createTexture("4");
	GLfloat floorVert2[12] = { 
		-1.0, 0.0,  3.0,
		 1.0, 0.0,  3.0,
		 1.0, 0.0, -3.0,
		-1.0, 0.0, -3.0};
	PlaneEntity* tmpFloor2 = createPlaneEntity("", stepTex, HORIZONTAL, floorVert2, 0, 2.0f, -7.0f);
	GLfloat floorVert3[12] = { 
		-1.0, 2.0,  0.0,
		 1.0, 2.0,  0.0,
		 1.0, -2.0, 0.0,
		-1.0, -2.0, 0.0};
	PlaneEntity* tmpFloor3 = createPlaneEntity("", stepTex, VERTICAL_X, floorVert3, 0, 0.0f, -4.0f);

		// Block
	BlockEntity* tmpBlock = createBlockEntity("", stepTex, 0.0f, 0.25f, 7.0f, 4.0f, 0.5f, 5.0f);

	//entities->add(tmpModel);
	entities->add(tmpWall1);
	entities->add(tmpWall2);
	entities->add(tmpFloor);
	//entities->add(tmpFloor2);
	//entities->add(tmpFloor3);
	entities->add(tmpBlock);
	entities->add(tmpShip);
	entities->add(wizard);
	entities->add(tmpCoin);
	entities->add(tmpGumballMachine);
	// ========== END TEST ========== //

	while( running ) {
		if( SDL_PollEvent(&event) ) {
			if(event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_QUIT) {
				running = false;
			} else if(event.type == SDL_KEYDOWN) {
				if(event.key.keysym.sym == SDLK_SPACE) player->jump();
				else if(event.key.keysym.sym == SDLK_x) player->turn180();
				keys[event.key.keysym.scancode] = true;
			} else if(event.type == SDL_KEYUP) {
				keys[event.key.keysym.scancode] = false;
			}
		}
		movePlayer(keys, player);

		// ========== START TEST ========== //
		player->incrementYOf(VELOCITY, -0.2f); // gravity

			// Must be  in level superclass: ******
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glLoadIdentity();

		glColor3f(1, 1, 1);

		GLfloat matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

#ifndef COLLIDED_CONSOLE_HERE
		if(player->hasCollided(tmpCoin)) {
			if(!collision[0]) {
				cout << "Collision: Coin" << endl;
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
#endif

		player->drawSelf(matrix, entities); // Used to adjust camera based on player position
		glLoadMatrixf(matrix);

		//skybox->drawSelf();
		entities->drawSelf();

			// ******************************* //
		// ========== END TEST ========== //
		
		SDL_GL_SwapWindow(mainWindow);

		printFPS(frames, initialTime);
	}
	
		// ========== START TEST ========== //
	delete entities;
	delete tmpWall1;
	delete tmpWall2;
	delete tmpFloor;
	delete tmpBlock;
	delete tmpShip;
	delete tmpCoin;
	delete tmpGumballMachine;

	delete wizard;
	delete player;
		// ========== END TEST ========== //
}

// Allows for movement in multiple directions and rotations simultaneously.
void movePlayer(bool* keys, PlayerEntity* player) {
		if(keys[SDL_GetScancodeFromKey(SDLK_UP)] || keys[SDL_GetScancodeFromKey(SDLK_w)]) player->moveForward(true);
		if(keys[SDL_GetScancodeFromKey(SDLK_DOWN)] || keys[SDL_GetScancodeFromKey(SDLK_s)]) player->moveForward(false);
		if(keys[SDL_GetScancodeFromKey(SDLK_q)] ) player->strafe(true);
		if(keys[SDL_GetScancodeFromKey(SDLK_e)] ) player->strafe(false);
		if(keys[SDL_GetScancodeFromKey(SDLK_LEFT)] || keys[SDL_GetScancodeFromKey(SDLK_a)]) player->rotate(true);
		if(keys[SDL_GetScancodeFromKey(SDLK_RIGHT)] || keys[SDL_GetScancodeFromKey(SDLK_d)]) player->rotate(false);
}

// Print the current FPS for debug purposes.
void printFPS(GLint &frames, GLint &initialTime) {
    frames++;
	GLint time = SDL_GetTicks();

	if (time - initialTime >= 5000) {
	    GLfloat seconds = (time - initialTime) / 1000.0;
	    GLfloat fps = frames / seconds;
	    printf("%d frames in %g seconds = %g FPS\n", frames, seconds, fps);
	    initialTime = time;
	    frames = 0;
	}
}
