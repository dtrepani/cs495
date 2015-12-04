#ifndef _COLLIDERNODE_
#define _COLLIDERNODE_

#include "Node.h"
#include "ColliderEntity.h"
#include "PlaneEntity.h"

class ColliderNode : public Node {
private:
	ColliderEntity* entity;
	ColliderNode* next;

	bool hasCollidedWhileIteratingThroughOtherList(ColliderNode* otherCollider);
	float distanceBetweenColliders(ColliderNode* otherCollider);
	float sumOfRadii(ColliderNode* otherCollider);

public:
	ColliderNode(ColliderEntity* anEntity, ColliderNode* aNext);
	~ColliderNode(void);
	bool hasCollidedWhileIteratingThroughThisList(ColliderNode* otherColliderHead);
	bool withinPlaneBoundaries(PlaneEntity* plane);
	ColliderNode* getNext();
	ColliderEntity* getEntity();
};

#endif
