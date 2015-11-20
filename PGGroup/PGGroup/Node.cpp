#include "Node.h"

Node::Node(Entity* anEntity, Node* aNext) {
	entity = anEntity;
	next = aNext;
}

Node::~Node(void) {
	delete next;
}

// Only for use with collider entities.
bool Node::hasCollided(Node* otherCollider) {
	Entity* otherEntity = otherCollider->getEntity();

	if( entity->getPosition()->distance(otherEntity->getPosition()) <= ( ((ColliderEntity*)entity)->getRadius() + ((ColliderEntity*)otherEntity)->getRadius() ) ) {
		return true;
	}

	if(next) {
		return next->hasCollided(otherCollider);
	}

	return false;
}

Entity* Node::getEntity() { return entity; }
