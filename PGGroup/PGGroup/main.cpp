/*
* Name:			
* Project:		PGGroup
* Description:	
*/
#include "main.h"

SDL_Window *mainWindow;
glQuaternion qAngle;

int main(int argc, char **argv) {
	init();
	pollEventsAndDraw();

	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
	return 0;
}

// Initialize variables and call initalizations for the window, openGL, and textures
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

// Better suited in the level superclass
// texture may be used when the same texture is needed multiple times throughout a level, such as for the aisles in the tutorial.
// In this case, create the texture beforehand and pass it in.
Entity* createEntity(string name, GLuint* texture, GLfloat* vertices, float x, float y, float z) { 
	return new Entity(new Vector(x, y, z), (texture == NULL) ? createTexture(name) : texture, vertices);
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
	
	// ========== START TEST ========== //
#ifdef cube
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
	GLfloat modelVert[12] = {
		-1.0, -1.0,  1.0,
		 1.0, -1.0,  1.0,
		 1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0};
	Entity* tmpModel = createEntity("1", NULL, modelVert, 0, 1.0f, -10.0f);
	tmpModel->addCollider(0, 0, 0, 0);
	tmpModel->incrementYOf(ROTATION, 20.0f);

	GLfloat floorVert[12] = { 
		-3.0, 0.0,  40.0,
		 3.0, 0.0,  40.0,
		 3.0, 0.0, -40.0,
		-3.0, 0.0, -40.0};
	Entity* tmpFloor = createEntity("2", NULL, floorVert, 0, 0, 0);

	GLuint* wallTex = createTexture("3");
	GLfloat wallVert[12] = { 
		0.0, 0.0,  40.0,
		0.0, 10.0,  40.0,
		0.0, 10.0, -40.0,
		0.0, 0.0, -40.0};
	Entity* tmpWall1 = createEntity("", wallTex, wallVert, -3.0f, 0, 0);
	Entity* tmpWall2 = createEntity("", wallTex, wallVert, 3.0f, 0, 0);

	PlayerEntity* player = createPlayerEntity(0, 1.0f, -6.0f);
	// ========== END TEST ========== //

	while( running ) {
		if( SDL_PollEvent(&event) ) {
			// check for other keys

			// ========== START TEST ========== //
			switch(event.key.keysym.sym) {
			case SDLK_w:
			case SDLK_UP:
				//player->incrementZOf(POSITION, 0.2f);
				player->moveForward(true);
				break;
			case SDLK_s:
			case SDLK_DOWN:
				//player->incrementZOf(POSITION, -0.1f);
				player->moveForward(false);
				break;
			case SDLK_q:
				//player->incrementXOf(POSITION, 0.1f);
				player->moveSideways(true);
				break;
			case SDLK_e:
				//player->incrementXOf(POSITION, -0.1f);
				player->moveSideways(false);
				break;
			case SDLK_a:
			case SDLK_LEFT:
				player->incrementYOf(ROTATION, 1.5f);
				break;
			case SDLK_d:
			case SDLK_RIGHT:
				player->incrementYOf(ROTATION, -1.5f);
				break;
			case SDLK_ESCAPE:
				running = false;
				break;
			}

			if(event.type == SDL_QUIT) running = false;
			// ========== END TEST ========== //
		}

		// ========== START TEST ========== //
		// Must be  in level superclass: ******
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glLoadIdentity();

		glColor3f(1, 1, 1);

		GLfloat matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		player->drawSelf(matrix); // Used to adjust camera based on player position
		glLoadMatrixf(matrix);
		// ******************************* //


		//tmpModel->incrementXOf(ROTATION, 5.0f);
		//tmpModel->incrementYOf(POSITION, 0.005f);

		if(player->hasCollided(tmpModel)) {
			cout << "Collision!" << endl;
		}
		
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
