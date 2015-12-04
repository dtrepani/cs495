#include "ColliderLinkedList.h"

ColliderLinkedList::ColliderLinkedList(void) { head = NULL; }

ColliderLinkedList::~ColliderLinkedList(void) {}

// Check if the any of the collision entities in the two collider entities have collided.
bool ColliderLinkedList::hasCollided(ColliderLinkedList* otherColliders) {
	return (head == NULL) ? false : head->hasCollidedWhileIteratingThroughThisList(otherColliders->getHead());
}

// Check if any of the collision entities are within the plane's boundaries.
bool ColliderLinkedList::withinPlaneBoundaries(Orientation orientation, float biggestPositionVal1, float smallestPositionVal1, float biggestPositionVal2, float smallestPositionVal2) {
	return (head == NULL) ? false : head->withinPlaneBoundaries(orientation, biggestPositionVal1, smallestPositionVal1, biggestPositionVal2, smallestPositionVal2);	
}

void ColliderLinkedList::add(ColliderEntity *anEntity) {
	head = new ColliderNode(anEntity, head);
}

ColliderNode* ColliderLinkedList::getHead() { return head; }
