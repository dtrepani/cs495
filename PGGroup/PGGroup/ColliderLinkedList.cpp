#include "ColliderLinkedList.h"

ColliderLinkedList::ColliderLinkedList(void) {}

ColliderLinkedList::~ColliderLinkedList(void) {}

// Check if the any of the collision entities in the two collider entities have collided.
bool ColliderLinkedList::hasCollided(ColliderLinkedList* otherColliders) {
	return head->hasCollidedWhileIteratingThroughThisList(otherColliders->getHead());
}
