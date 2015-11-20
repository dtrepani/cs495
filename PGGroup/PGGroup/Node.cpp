#include "Node.h"


Node::Node(Entity *anEntity, Node *aNext) {
	entity = anEntity;
	next = aNext;
}


Node::~Node(void) {
	delete next;
}
