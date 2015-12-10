/*
* Name:			
* Project:		PGGroup
* Description:	
*/
#include "main.h"

SDL_Window *mainWindow;
LinkedList* entities, platforms;

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
	mainWindow = SDL_CreateWindow(	"GumballZ",
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
	//glClearColor(0.35f, 0.6f, 1, 1);
	glClearColor(0.1f, 0.1f, 0.1f, 1);
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
	Entity* entity = new Entity(new Vector(x, y, z), (texture == NULL) ? createTexture(name) : texture, vertices, radius);
	entities->add(entity);
	return entity;
}
PlaneEntity* createPlaneEntity(string name, GLuint* texture, Orientation orientation, GLfloat* vertices, float x, float y, float z) {
	PlaneEntity* entity = new PlaneEntity(new Vector(x, y, z), (texture == NULL) ? createTexture(name) : texture, vertices, orientation);
	entities->add(entity);
	return entity;
}
BlockEntity* createBlockEntity(string name, GLuint* texture, float x, float y, float z, LinkedList* list, float widthX, float heightY, float lengthZ) { // list is the list the entity should be added to
	BlockEntity* entity = new BlockEntity(new Vector(x, y, z), (texture == NULL) ? createTexture(name) : texture, widthX, heightY, lengthZ);
	list->add(entity);
	return entity;
}
InteractableEntity* createInteractableEntity(string name, GLuint* texture, GLfloat* vertices, float radius, float x, float y, float z) { 
	InteractableEntity* entity = new InteractableEntity(new Vector(x, y, z), (texture == NULL) ? createTexture(name) : texture, vertices, radius);
	entities->add(entity);
	return entity;
}
WizardEntity* createWizardEntity(string name, GLfloat* vertices, float radius, PlayerEntity* player, float x, float y, float z, LinkedList* covers, LinkedList* platforms) { 
	WizardEntity* entity = new WizardEntity(new Vector(x, y, z), createTexture(name), vertices, radius, player, covers, platforms);
	entities->add(entity);
	return entity;
}
PlayerEntity* createPlayerEntity(float x, float y, float z, float radius) { 
	return new PlayerEntity(new Vector(x, y, z), radius);
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

	// ========== START LEVEL ========== //
	entities = new LinkedList();
	// To be passed into the wizard class
	LinkedList* covers = new LinkedList();
	LinkedList* platforms = new LinkedList();
	
	PlayerEntity* player = createPlayerEntity(0, 1.0f, 0, NULL);

	float blockDepth = 0.4f;
	float lvlWidth  = 4.0f,
		  lvlHeight = 7.0f;
	// [Start of plane, length of plane]
	float lvlLength[] = { 10.0f, -40.0f, // P1
						  -80.0f, -40.0f,  // P2
						  -30.0f, -50.0f  // Cover
						 };

	GLuint* texSpaceshipWall = createTexture("spaceship_wall");
	GLuint* texSpaceShipFloor = createTexture("spaceship_floor");

	// Walls use y+blockDepth for their heightY param to create a a smooth appearance to the outside of the ship
	// Floors may use z+0.01f for their lengthZ param to prevent clipping with walls

	// FRIST PART
		// Walls
	BlockEntity* spaceshipWallLeft = createBlockEntity("", texSpaceshipWall, -lvlWidth,		lvlHeight/2.0f,			lvlLength[0]+lvlLength[1]/2.0f, entities,
																			  blockDepth,	lvlHeight+blockDepth,	lvlLength[1]);
	BlockEntity* spaceshipWallRight = createBlockEntity("", texSpaceshipWall, lvlWidth,		lvlHeight/2.0f,			lvlLength[0]+lvlLength[1]/2.0f, entities,
																			  blockDepth,	lvlHeight+blockDepth,	lvlLength[1]);
	BlockEntity* spaceshipWallBack = createBlockEntity("", texSpaceshipWall, 0,				lvlHeight/2.0f,			lvlLength[0], entities,
																			 lvlWidth*2.0f,	lvlHeight+blockDepth,	blockDepth);

		// Floors
	BlockEntity* spaceshipFloor = createBlockEntity("", texSpaceShipFloor,	0,				0,			lvlLength[0]+lvlLength[1]/2.0f, entities,
																			lvlWidth*2.0f,	blockDepth,	lvlLength[1]+0.01f);
	BlockEntity* spaceshipCeiling = createBlockEntity("", texSpaceshipWall,	0,				lvlHeight,	lvlLength[0]+lvlLength[1]/2.0f, entities,
																			lvlWidth*2.0f,	blockDepth,	lvlLength[1]+0.01f);

	// JP COVER
		// Walls
	BlockEntity* spaceshipWallLeftCover = createBlockEntity("", texSpaceshipWall,	-lvlWidth,		lvlHeight/2.0f,			lvlLength[4]+lvlLength[5]/2.0f, covers,
																					blockDepth,		lvlHeight+blockDepth,	lvlLength[5]);
	BlockEntity* spaceshipWallRightCover = createBlockEntity("", texSpaceshipWall,	lvlWidth,		lvlHeight/2.0f,			lvlLength[4]+lvlLength[5]/2.0f, covers,
																					blockDepth,		lvlHeight+blockDepth,	lvlLength[5]);

		// Floors
	BlockEntity* spaceshipFloorCover = createBlockEntity("", texSpaceShipFloor,	0,				0,			lvlLength[4]+lvlLength[5]/2.0f, covers,
																				lvlWidth*2.0f,	blockDepth,	lvlLength[5]+0.01f);
	BlockEntity* spaceshipCeilingCover = createBlockEntity("", texSpaceshipWall,0,				lvlHeight,	lvlLength[4]+lvlLength[5]/2.0f, covers,
																				lvlWidth*2.0f,	blockDepth,	lvlLength[5]+0.01f);

	// JP
	float prevPlatZ = lvlLength[0]+lvlLength[1];
	BlockEntity* platform1 = createBlockEntity("", texSpaceShipFloor,	1.8f,	0.5f,		prevPlatZ=(prevPlatZ-4.0f), platforms,
																		3.0f,	blockDepth,	4.0f);
	platform1->setRandomRotation();
	BlockEntity* platform2 = createBlockEntity("", texSpaceShipFloor,	-2.2f,	0.0f,		prevPlatZ=(prevPlatZ-6.0f), platforms,
																		4.0f,	blockDepth,	3.0f);
	platform2->setRandomRotation();
	BlockEntity* platform3 = createBlockEntity("", texSpaceShipFloor,	0.0f,	0.0f,		prevPlatZ=(prevPlatZ-7.0f), platforms,
																		3.0f,	blockDepth,	3.0f);
	platform3->setRandomRotation();
	BlockEntity* platform4 = createBlockEntity("", texSpaceShipFloor,	4.5f,	2.0f,		prevPlatZ=(prevPlatZ-6.0f), platforms,
																		3.0f,	blockDepth,	3.0f);
	platform4->setRandomRotation();
	BlockEntity* platform5 = createBlockEntity("", texSpaceShipFloor,	-4.0f,	3.0f,		prevPlatZ-6.0f, platforms,
																		3.0f,	blockDepth,	3.0f);
	platform5->setRandomRotation();

		// Downward fork
	BlockEntity* platform6 = createBlockEntity("", texSpaceShipFloor,	-3.0f,	-1.0f,		prevPlatZ=(prevPlatZ+1.0f), platforms,
																		3.0f,	blockDepth,	3.0f);
	platform6->setRandomRotation();
	BlockEntity* platform7 = createBlockEntity("", texSpaceShipFloor,	0.0f,	-2.0f,		prevPlatZ=(prevPlatZ-5.0f), platforms,
																		3.0f,	blockDepth,	3.0f);
	platform7->setRandomRotation();
	BlockEntity* platform8 = createBlockEntity("", texSpaceShipFloor,	2.0f,	-3.0f,		prevPlatZ=(prevPlatZ-6.0f), platforms,
																		3.0f,	blockDepth,	3.0f);
	platform8->setRandomRotation();
	
	BlockEntity* platform9 = createBlockEntity("", texSpaceShipFloor,	0.0f,	-1.0f,		prevPlatZ=(prevPlatZ-5.0f), platforms,
																		3.0f,	blockDepth,	3.0f);
	platform9->setRandomRotation();
	BlockEntity* platform10 = createBlockEntity("", texSpaceShipFloor,	-4.0f,	0.0f,		prevPlatZ-7.0f, platforms,
																		3.0f,	blockDepth,	3.0f);
	platform10->setRandomRotation();
	BlockEntity* platform11 = createBlockEntity("", texSpaceShipFloor,	2.0f,	1.5f,		prevPlatZ-6.0f, platforms,
																		3.0f,	blockDepth,	3.0f);
	platform11->setRandomRotation();


	// SECOND PART
		// Walls
	BlockEntity* spaceshipWallLeft2 = createBlockEntity("", texSpaceshipWall, -lvlWidth,		lvlHeight/2.0f,			lvlLength[2]+lvlLength[3]/2.0f, entities,
																			   blockDepth,		lvlHeight+blockDepth,	lvlLength[3]);
	BlockEntity* spaceshipWallRight2 = createBlockEntity("", texSpaceshipWall, lvlWidth,		lvlHeight/2.0f,			lvlLength[2]+lvlLength[3]/2.0f, entities,
																			   blockDepth,		lvlHeight+blockDepth,	lvlLength[3]);
	BlockEntity* spaceshipWallFront = createBlockEntity("", texSpaceshipWall, 0,				lvlHeight/2.0f,			lvlLength[2]+lvlLength[3], entities,
																			  lvlWidth*2.0f,	lvlHeight+blockDepth,	blockDepth);

		// Floors
	BlockEntity* spaceshipFloor2 = createBlockEntity("", texSpaceShipFloor,	0,				0,			lvlLength[2]+lvlLength[3]/2.0f, entities,
																			lvlWidth*2.0f,	blockDepth,	lvlLength[3]+0.01f);
	BlockEntity* spaceshipCeiling2 = createBlockEntity("", texSpaceshipWall,0,				lvlHeight,	lvlLength[2]+lvlLength[3]/2.0f, entities,
																			lvlWidth*2.0f,	blockDepth,	lvlLength[3]+0.01f);
	

	// NPCS & INTERACTABLES ===========================
	GLfloat gumballMachineVert[12] = { 
		-0.3f, -2.4f*0.3f,  0.0,
		 0.3f, -2.4f*0.3f,  0.0,
		 0.3f, 2.4f*0.3f, 0.0,
		-0.3f, 2.4f*0.3f, 0.0};
	InteractableEntity* gumballMachine = new InteractableEntity(new Vector(0, 0.65f, lvlLength[2]+lvlLength[3]+2.0f), createTexture("gumballmachine"), gumballMachineVert, NULL);
	entities->add(gumballMachine);

	GLfloat wizardVert[12] = {
		-0.4f, -850.0f/400.0f*0.4f,  0,
		 0.4f, -850.0f/400.0f*0.4f,  0,
		 0.4f,  850.0f/400.0f*0.4f,  0,
		-0.4f,  850.0f/400.0f*0.4f,  0};
	WizardEntity* wizard = createWizardEntity("wizard", &wizardVert[0], 0.0f, player, 0, 1.5f, -7.0f, covers, platforms);

	GLfloat coinVert[12] = { 
		-0.25f, -25.0f/75.0f*0.25f,  0.0,
		 0.25f, -25.0f/75.0f*0.25f,  0.0,
		 0.25f, 25.0f/75.0f*0.25f, 0.0,
		-0.25f, 25.0f/75.0f*0.25f, 0.0};
	CoinInteractableEntity* coin = new CoinInteractableEntity(new Vector(0, 0.1f, -5.0f), createTexture("coin"), coinVert, 1.5f);
	entities->add(coin);
	
	
	// ========== END LEVEL ========== //

	while( running ) {
		if( SDL_PollEvent(&event) ) {
			if(event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_QUIT) {
				running = false;
			} else if(event.type == SDL_KEYDOWN) {
				if(event.key.keysym.sym == SDLK_SPACE) player->jump();
				else if(event.key.keysym.sym == SDLK_x) player->turn180();
				else if(event.key.keysym.sym == SDLK_f) player->interactWith();
				keys[event.key.keysym.scancode] = true;
			} else if(event.type == SDL_KEYUP) {
				keys[event.key.keysym.scancode] = false;
			}
		}
		movePlayer(keys, player);

		// ========== START TEST ========== //
		player->incrementYOf(VELOCITY, -0.2f); // gravity

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glLoadIdentity();
		glColor3f(1, 1, 1);

		GLfloat matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		player->drawSelf(matrix, entities, platforms); // Used to adjust camera based on player position
		glLoadMatrixf(matrix);

		if(player->isDead()) {
			wizard->reset();
		}
		entities->drawSelf();
		// ========== END TEST ========== //
		
		SDL_GL_SwapWindow(mainWindow);

		printFPS(frames, initialTime);
	}
	
		// ========== START TEST ========== //
	delete entities;
	delete covers;
	delete platforms;
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
