#include "ColliderNode.h"

ColliderNode::ColliderNode(ColliderEntity* anEntity, ColliderNode* aNext) : Node(anEntity, aNext) {}

ColliderNode::~ColliderNode(void) {}

// Iterate through this list's nodes to check for collision
bool ColliderNode::hasCollidedWhileIteratingThroughThisList(ColliderNode* otherColliderHead) {
	if(hasCollidedWhileIteratingThroughThisList(otherColliderHead)) { return true; }
	if(next) { return next->hasCollidedWhileIteratingThroughThisList(otherColliderHead); }
	return false;
}

// Iterate through the other collider list's nodes to check for collision
bool ColliderNode::hasCollidedWhileIteratingThroughOtherList(ColliderNode* otherCollider) {
	if(distanceBetweenColliders(otherCollider) <= sumOfRadii(otherCollider)) { return true;	}
	if(otherCollider->getNext()) { return hasCollidedWhileIteratingThroughOtherList(otherCollider->getNext()); }
	return false;
}

// Distance between the centers of this node's collider and the other node's collider
float ColliderNode::distanceBetweenColliders(ColliderNode* otherCollider) {
	ThreeAxis* colliderPos1 = entity->getPosition();
	ThreeAxis* colliderPos2 = otherCollider->getEntity()->getPosition();

	float distance = colliderPos1->distanceTo(colliderPos2);

	delete colliderPos1;
	delete colliderPos2;

	return distance;
}

// Sum of the radii of this node's collider and the other node's collider
int ColliderNode::sumOfRadii(ColliderNode* otherCollider) {
	ColliderEntity* otherEntity = otherCollider->getEntity();
	return	( entity->getRadius() + otherEntity->getRadius() );
}

