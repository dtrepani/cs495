#include "ThreeAxis.h"

ThreeAxis::ThreeAxis(float anX, float aY, float aZ) { x = anX; y = aY; z = aZ; }

ThreeAxis::~ThreeAxis(void) {}

// Determine the distance between two entities.
// Only an entity's position should be passed in.
float ThreeAxis::distance(ThreeAxis* otherEntity) {
	return sqrt(pow((otherEntity->getX()-x), 2) + pow((otherEntity->getY()-y), 2) + pow((otherEntity->getZ()-z), 2));
}

float ThreeAxis::getX() { return x; }
float ThreeAxis::getY() { return y; }
float ThreeAxis::getZ() { return z; }
void ThreeAxis::setX(float anX) { x = anX; }
void ThreeAxis::setY(float aY) { y = aY; }
void ThreeAxis::setZ(float aZ) { z = aZ; }
