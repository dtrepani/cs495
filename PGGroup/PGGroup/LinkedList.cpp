#include "LinkedList.h"


LinkedList::LinkedList(void) {
	head = NULL;
}

LinkedList::~LinkedList(void) {
	delete head;
}

void LinkedList::add(Entity *anEntity) {
	head = new Node(anEntity, head);
}

bool LinkedList::hasCollided(LinkedList* otherColliders) {
	return head->hasCollided(otherColliders->getHead());
}

Node* LinkedList::getHead() { return head; };
