#ifndef _THREEAXIS_
#define _THREEAXIS_

#include <cmath>

class ThreeAxis {
private:
	float x, y, z;

public:
	ThreeAxis(float anX, float aY, float aZ);
	~ThreeAxis(void);

	float distanceTo(ThreeAxis* otherEntity);

	float getX();
	float getY();
	float getZ();
	void setX(float anX);
	void setY(float aY);
	void setZ(float aZ);
};

#endif
