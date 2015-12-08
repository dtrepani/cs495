#include "Level.h"
#include <math.h>


Level::Level(string filename, PlayerEntity* aPlayer){
	player = aPlayer;
	planeList = new LinkedList();
	interactableList = new LinkedList();
	ifstream *levelfile = new ifstream(filename);
	string s;
	while(1){
		getline(*levelfile,s);
		if(levelfile->eof())
			break;
		if(s.compare("NEW PLANE") == 0){
			planeList->add(createPlaneEntity(levelfile));
		}
		if(s.compare("NEW INTERACTABLE") == 0){

		}
		if(s.compare("WIZARD") == 0){

		}
		if(s.compare("NEW COIN") == 0){

		}
		if(s.compare("PLAYER") == 0){

		}
	}
	levelfile->close();
	delete levelfile;
}
Level::~Level(){
	delete player;
	delete planeList;
	delete interactableList;
	delete wizard;
}
GLuint* Level ::createTexture(string name) {
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
int Level::stringToInt(string s){
	int ret = 0;
	int p = 0;
	for(int i = s.length()-1; i >=0; i--){
		ret += (int)((s[i]-'0')*pow(10,p));
		p++;
	}
	return ret;
}
GLfloat Level::stringToGLfloat(string s){
	GLfloat ret = 0.0;
	int len = s.length();
	int count =0;
	int dcount = 0;
	int i;
	for(i = len-1;i>=0;i--){
		if(s[i] == '.'){
			dcount = count;
			continue;
		}
		if(s[i] == '-'){
			ret = -ret;
			break;
		}
		ret += (GLfloat)((s[i]-'0')*pow(10,count));
		count++;
	}
	
	ret = ret*pow(10,-dcount);
	return ret;
}
PlaneEntity* Level:: createPlaneEntity(ifstream *f) { 
	string s;
	getline(*f,s);
	float x = (float)stringToInt(s);
	getline(*f,s);
	float y = (float)stringToInt(s);
	getline(*f,s);
	float z = (float)stringToInt(s);

	string texture;
	getline(*f,texture);

	GLfloat vertices[12];
	for(int i = 0; i <12; i++){
		getline(*f,s);
		vertices[i] = stringToGLfloat(s);
	}

	Orientation orientation;
	getline(*f,s);
	if(s.compare("HORIZONTAL") ==0)
		orientation = HORIZONTAL;
	if(s.compare("VERTICAL_X") ==0)
		orientation = VERTICAL_X;
	if(s.compare("VERTICAL_Z") ==0)
		orientation = VERTICAL_Z;
	PlaneEntity* ret = new PlaneEntity(new Vector(x, y, z), createTexture(texture), &vertices[0], orientation);
	return ret;
}

void Level:: drawEveryThing(){
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	glColor3f(1, 1, 1);

	GLfloat matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	//player->drawSelf(matrix); // Used to adjust camera based on player position
	glLoadMatrixf(matrix);

	//planeList->drawContents();
	
}

void Level:: collisionCheck(){
	//planeList->collisionCheck(player);
}