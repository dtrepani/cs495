/*
* Name:			
* Project:		PGGroup
* Description:	
*/
#include "main.h"

SDL_Window *mainWindow;
//SDL_Surface **textureSurface;
//GLuint *texture;
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
Entity* createEntity(string name, GLfloat* vertices, float x, float y, float z) { return new Entity(new ThreeAxis(x, y, z), createTexture(name), vertices); }
InteractableEntity* createInteractableEntity(string name, GLfloat* vertices, float x, float y, float z) { return new InteractableEntity(new ThreeAxis(x, y, z), createTexture(name), vertices); }
PlayerEntity* createPlayerEntity(float x, float y, float z) { return new PlayerEntity(new ThreeAxis(x, y, z), NULL, NULL); }
WizardEntity* createWizardEntity(string name, GLfloat* vertices, float x, float y, float z) { return new WizardEntity(new ThreeAxis(x, y, z), createTexture(name), vertices); }

// Each texture being created goes through the same method calls and is named with a number, referenced by index.
GLuint* createTexture(string name) {
	GLuint* texture = new GLuint;
	SDL_Surface* textureSurface = IMG_Load( ("resources\\" + name + ".jpg").c_str() ); // TODO: Change back to SDLLoad (?) and .bmp
	if(textureSurface == NULL) cout << "IMG_Load: " << SDL_GetError() << endl;

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
	GLfloat vertices[12] = { 1,1,0,   -1,1,0,   -1,-1,0,   1,-1,0,}; // default plane

	Entity* tmpModel = createEntity("tmp", vertices, 0, 0, -10.0f);
	tmpModel->addCollider(0, 0, 0, 0);
	PlayerEntity* player = createPlayerEntity(0, 0, 0);
	// ========== END TEST ========== //

	while( running ) {
		if( SDL_PollEvent(&event) ) {
			// check for other keys

			// ========== START TEST ========== //
			switch(event.key.keysym.sym) {
			case SDLK_w:
			case SDLK_UP:
				player->incrementZOf(POSITION, 0.2f);
				break;
			case SDLK_s:
			case SDLK_DOWN:
				player->incrementZOf(POSITION, -0.1f);
				break;
			case SDLK_q:
				player->incrementXOf(POSITION, 0.1f);
				break;
			case SDLK_e:
				player->incrementXOf(POSITION, -0.1f);
				break;
			case SDLK_a:
			case SDLK_LEFT:
				player->incrementYOf(ROTATION, -0.4f);
				break;
			case SDLK_d:
			case SDLK_RIGHT:
				player->incrementYOf(ROTATION, 0.4f);
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

		// TO-DO: 
		//	Fix collisions
		//	Fix rotation (convert to quaternions?)

		tmpModel->drawSelf();

		// ========== END TEST ========== //
		
		SDL_GL_SwapWindow(mainWindow);
	}
}
