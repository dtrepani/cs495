#ifndef _VECTOR
#define _VECTOR_

#include <cmath>

class Vector {
private:
	float x, y, z;

public:
	Vector(float anX, float aY, float aZ);
	~Vector(void);

	float distanceTo(Vector* otherEntity);
	bool equalsWithError(Vector* other, float errorThreshold);
	void setEqualTo(Vector* other);
	void zero();
	float dotProduct(Vector* other);

	Vector* add(Vector* other);
	Vector* subtract(Vector* other);
	Vector* scalarMultiply(float scalar);
	Vector* normalize();
	Vector* crossProduct(Vector* other);

	void incrementX(float anX);
	void incrementY(float aY);
	void incrementZ(float aZ);

	float getX();
	float getY();
	float getZ();
	void setX(float anX);
	void setY(float aY);
	void setZ(float aZ);
};

#endif
