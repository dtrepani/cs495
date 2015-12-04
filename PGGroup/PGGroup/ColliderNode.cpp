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


bool ColliderNode::withinPlaneBoundaries(Orientation orientation, float biggestPositionVal1, float smallestPositionVal1, float biggestPositionVal2, float smallestPositionVal2) {
	bool within = false;
	Vector* colliderPos = entity->getPosition();
	float colliderRadius = entity->getRadius();

	if(next) within = next->withinPlaneBoundaries(orientation, biggestPositionVal1, smallestPositionVal1, biggestPositionVal2, smallestPositionVal2);
	if( (orientation == VERTICAL_X) &&
				(colliderPos->getX()+colliderRadius < biggestPositionVal1 && colliderPos->getX()-colliderRadius > smallestPositionVal1) &&
				(colliderPos->getY()+colliderRadius < biggestPositionVal2 && colliderPos->getY()-colliderRadius > smallestPositionVal2) ) {
					within = true;
	} else if ( (orientation == VERTICAL_Z) &&
				(colliderPos->getY()+colliderRadius < biggestPositionVal1 && colliderPos->getY()-colliderRadius > smallestPositionVal1) &&
				(colliderPos->getZ()+colliderRadius < biggestPositionVal2 && colliderPos->getZ()-colliderRadius > smallestPositionVal2) ) {
					within = true;
	} else if ( (orientation == HORIZONTAL) &&
				(colliderPos->getX()+colliderRadius < biggestPositionVal1 && colliderPos->getX()-colliderRadius > smallestPositionVal1) &&
				(colliderPos->getZ()+colliderRadius < biggestPositionVal2 && colliderPos->getZ()-colliderRadius > smallestPositionVal2) ) {
					within = true;
	}

	return within;
}

ColliderNode* ColliderNode::getNext() { return next; }
ColliderEntity* ColliderNode::getEntity() { return entity; }

