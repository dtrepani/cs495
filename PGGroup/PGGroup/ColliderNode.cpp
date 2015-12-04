#include "ColliderNode.h"

ColliderNode::ColliderNode(ColliderEntity* anEntity, ColliderNode* aNext) : Node(anEntity, aNext) {
	entity = anEntity;
	next = aNext;
}

ColliderNode::~ColliderNode(void) {}

// Iterate through this list's nodes to check for collision
bool ColliderNode::hasCollidedWhileIteratingThroughThisList(ColliderNode* otherColliderHead) {
	if(hasCollidedWhileIteratingThroughOtherList(otherColliderHead)) { return true; }
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
	Vector* colliderPos1 = entity->getPosition();
	Vector* colliderPos2 = otherCollider->getEntity()->getPosition();

	float distance = colliderPos1->distanceTo(colliderPos2);

	delete colliderPos1;
	delete colliderPos2;

	return distance;
}

// Sum of the radii of this node's collider and the other node's collider
float ColliderNode::sumOfRadii(ColliderNode* otherCollider) {
	ColliderEntity* otherEntity = otherCollider->getEntity();
	return ( entity->getRadius() + otherEntity->getRadius() );
}


bool ColliderNode::withinPlaneBoundaries(PlaneEntity* plane) {
	bool within = false;
	Orientation planeOrientation = plane->getOrientation();
	Vector* colliderPos = entity->getPosition();
	float colliderRadius = entity->getRadius();

	if(next) within = next->withinPlaneBoundaries(plane);
	if( (planeOrientation == VERTICAL_X) &&
				(colliderPos->getX()-colliderRadius < plane->getMax(X) && colliderPos->getX()+colliderRadius > plane->getMin(X)) &&
				(colliderPos->getY()+colliderRadius < plane->getMax(Y) && colliderPos->getY()-colliderRadius > plane->getMin(Y)) ) {
					within = true;
	} else if ( (planeOrientation == VERTICAL_Z) &&
				(colliderPos->getY()+colliderRadius < plane->getMax(Y) && colliderPos->getY()-colliderRadius > plane->getMin(Y)) &&
				(colliderPos->getZ()+colliderRadius < plane->getMax(Z) && colliderPos->getZ()-colliderRadius > plane->getMin(Z)) ) {
					within = true;
	} else if ( (planeOrientation == HORIZONTAL) &&
				(colliderPos->getX()+colliderRadius < plane->getMax(X) && colliderPos->getX()-colliderRadius > plane->getMin(X)) &&
				(colliderPos->getZ()+colliderRadius < plane->getMax(Z) && colliderPos->getZ()-colliderRadius > plane->getMin(Z)) ) {
					within = true;
	}

	return within;
}

ColliderNode* ColliderNode::getNext() { return next; }
ColliderEntity* ColliderNode::getEntity() { return entity; }

