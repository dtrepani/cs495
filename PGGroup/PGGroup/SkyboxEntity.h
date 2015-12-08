#ifndef _SKYBOXENTITY_
#define _SKYBOXENTITY_

#include "BlockEntity.h"

class SkyboxBlockEntity : public BlockEntity {
private:
	// Skybox has six textures.
	// Front = 0, Back = 1, Top = 2, Bottom = 3, Right = 4, and Left = 5
	int textureNum;

public:
	SkyboxBlockEntity(void);
	~SkyboxBlockEntity(void);
	void createAndAdd(GLfloat* vertices, Orientation orientation);
};

#endif
