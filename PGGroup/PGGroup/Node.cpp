#include "Node.h"
#include "Entity.h"

Node::Node(Entity* anEntity, Node* aNext) {
	entity = anEntity;
	next = aNext;
}

Node::~Node(void) {
	delete next;
}

Node* Node::getNext() { return next; }
Entity* Node::getEntity() { return entity; }

bool Node::hasCollided(Entity* otherEntity) {
	if(entity->hasCollided(otherEntity)) return true;
	if(next) return next->hasCollided(otherEntity);
	return false;
}

bool Node::isMovingToward(Entity* otherEntity) {
	if(entity->isMovingToward(otherEntity)) return true;
	if(next) return next->hasCollided(otherEntity);
	return false;
}

bool Node::checkForCollision(Entity* otherEntity) {
	bool collision = false;

	if(next) collision = next->checkForCollision(otherEntity);
	if(entity->checkForCollision(otherEntity)) collision = true;

	return collision;
}

void Node::drawSelf() {
	if(next) next->drawSelf();
	entity->drawSelf();
}
