#include "ColliderLinkedList.h"

ColliderLinkedList::ColliderLinkedList(void) { head = NULL; }

ColliderLinkedList::~ColliderLinkedList(void) {}

// Check if the any of the collision entities in the two collider entities have collided.
bool ColliderLinkedList::hasCollided(ColliderLinkedList* otherColliders) {
	return (head == NULL) ? false : head->hasCollidedWhileIteratingThroughThisList(otherColliders->getHead());
}

void ColliderLinkedList::add(ColliderEntity *anEntity) {
	head = new ColliderNode(anEntity, head);
}

ColliderNode* ColliderLinkedList::getHead() { return head; }
