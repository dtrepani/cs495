#include "Node.h"

Node::Node(Entity* anEntity, Node* aNext) {
	entity = anEntity;
	next = aNext;
}

Node::~Node(void) {
	delete next;
}

Node* Node::getNext() { return next; }
Entity* Node::getEntity() { return entity; }
