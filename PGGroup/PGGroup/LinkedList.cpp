#include "LinkedList.h"

LinkedList::LinkedList(void) {
	head = NULL;
}

LinkedList::~LinkedList(void) {
	delete head;
}

void LinkedList::add(Entity* anEntity) {
	head = new Node(anEntity, head);
}

Node* LinkedList::getHead() { return head; };

bool LinkedList::hasCollided(Entity* otherEntity) {
	if(head) return head->hasCollided(otherEntity);
	return false;
}

bool LinkedList::isMovingToward(Entity* otherEntity) {
	if(head) return head->isMovingToward(otherEntity);
	return false;
}

bool LinkedList::checkForCollision(Entity* otherEntity) {
	if(head) return head->checkForCollision(otherEntity);
	return false;
}

void LinkedList::drawSelf() {
	if(head) head->drawSelf();
}

float LinkedList::getMin(int axis) {
	if(head) return head->getMin(axis);
	return 9999.9f;
}

float LinkedList::getMax(int axis) {
	if(head) return head->getMax(axis);
	return -9999.9f;
}
